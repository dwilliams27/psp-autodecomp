#!/usr/bin/env python3
"""
Extract files from Days of Thunder PSP DATA.PAK archives.

PAK Format (little-endian):
  Header (16 bytes at offset 0):
    u32 table_total_size   - Total size of entry table + string table
    u32 entries_size       - Size of the entry table portion (in bytes)
    u32 version            - Format version (1)
    u32 table_offset       - File offset where the entry table starts (typically 0x800)

  Entry Table (at table_offset, entries_size bytes):
    Depth-first recursive directory tree encoded as an array of u32 values.

    Each directory node consists of:
      Directory header (6 x u32):
        u32 name_str_offset  - Offset into string table for directory name
        u32 data_sector      - Starting sector (2048-byte blocks) for this dir's files
        u32 num_files        - Number of file entries directly in this directory
        u32 reserved0        - Always 0
        u32 num_subdirs      - Number of immediate subdirectories
        u32 reserved1        - Always 0

      Followed by num_files file entries (2 x u32 each):
        u32 name_str_offset  - Offset into string table (bit 31 set = hashed name)
        u32 file_size        - Size of the file data in bytes

      Followed by num_subdirs subdirectory HEADERS (6 x u32 each):
        (Same format as the directory header above, listed consecutively)

      Followed by the CONTENT of each subdirectory in order:
        Each subdirectory's content = its file entries + its sub-subdir headers
        + recursively their content. This creates a depth-first traversal.

    File naming: files with bit 31 set in name_str_offset use hash-based naming.
    The string table entry for such files is: 8 bytes hash + 3 byte ASCII extension + null.
    Extracted as "<hash_hex>.<ext>".

  String Table (at table_offset + entries_size):
    Null-terminated strings referenced by name_str_offset values.

  File Data:
    Sector-aligned (2048-byte sectors). A directory's data_sector field gives the
    starting sector for its first file. Files within a directory are packed
    consecutively, each rounded up to the next sector boundary.
"""

import struct
import math
import os
import sys
import argparse

SECTOR_SIZE = 2048


def read_header(f):
    """Read and return the PAK header fields."""
    f.seek(0)
    data = f.read(16)
    table_total_size, entries_size, version, table_offset = struct.unpack('<4I', data)
    return {
        'table_total_size': table_total_size,
        'entries_size': entries_size,
        'version': version,
        'table_offset': table_offset,
    }


def load_tables(f, header):
    """Load the entry table and string table into memory."""
    f.seek(header['table_offset'])
    entry_data = f.read(header['entries_size'])
    str_size = header['table_total_size'] - header['entries_size']
    str_data = f.read(str_size)
    return entry_data, str_data


def get_string(str_data, offset):
    """Read a null-terminated ASCII string from the string table."""
    if offset >= len(str_data):
        return f"<invalid_offset_{offset}>"
    end = str_data.index(0, offset) if 0 in str_data[offset:offset + 256] else offset + 256
    raw = str_data[offset:end]
    try:
        return raw.decode('ascii')
    except UnicodeDecodeError:
        return raw.decode('latin-1')


def get_file_name(str_data, raw_name_offset):
    """Decode a file name from the string table.

    If bit 31 is set, the entry uses hash-based naming (8 bytes hash + 3 byte ext).
    Otherwise it's a plain string offset.
    """
    is_hashed = bool(raw_name_offset & 0x80000000)
    str_offset = raw_name_offset & 0x7FFFFFFF

    if not is_hashed:
        return get_string(str_data, str_offset)

    # Hash-based name: 8 bytes of hash data followed by 3-byte extension
    if str_offset + 11 <= len(str_data):
        hash_bytes = str_data[str_offset:str_offset + 8]
        ext_bytes = str_data[str_offset + 8:str_offset + 11]
        try:
            ext = ext_bytes.decode('ascii')
            if ext.isprintable() and len(ext) == 3:
                return f"{hash_bytes.hex()}.{ext}"
        except UnicodeDecodeError:
            pass

    # Fallback: try as plain string
    return get_string(str_data, str_offset)


def parse_pak(entry_data, str_data):
    """Parse the entire PAK entry table and return a list of (path, sector, size) tuples."""
    u32s = list(struct.unpack(f'<{len(entry_data) // 4}I', entry_data))
    all_files = []

    def parse_dir_content(idx, path, nfiles, nsubs, data_sector):
        """Parse a directory's content: file entries, subdir headers, then subdir content."""
        current_sector = data_sector

        # 1. Read file entries (2 x u32 each)
        for _ in range(nfiles):
            raw_name = u32s[idx]
            file_size = u32s[idx + 1]
            idx += 2
            name = get_file_name(str_data, raw_name)
            fpath = f"{path}/{name}" if path else name
            all_files.append((fpath, current_sector, file_size))
            if file_size > 0:
                current_sector += math.ceil(file_size / SECTOR_SIZE)

        # 2. Read all subdirectory headers at this level (6 x u32 each)
        subdirs = []
        for _ in range(nsubs):
            sname_off = u32s[idx]
            ssector = u32s[idx + 1]
            snfiles = u32s[idx + 2]
            # u32s[idx + 3] is reserved (0)
            snsubs = u32s[idx + 4]
            # u32s[idx + 5] is reserved (0)
            idx += 6

            sname = get_string(str_data, sname_off)
            spath = f"{path}/{sname}" if path else sname
            subdirs.append((spath, ssector, snfiles, snsubs))

        # 3. Recursively parse each subdirectory's content (depth-first)
        for spath, ssector, snfiles, snsubs in subdirs:
            idx = parse_dir_content(idx, spath, snfiles, snsubs, ssector)

        return idx

    # Parse root directory header
    root_name_off = u32s[0]
    root_sector = u32s[1]
    root_nfiles = u32s[2]
    # u32s[3] reserved
    root_nsubs = u32s[4]
    # u32s[5] reserved

    root_name = get_string(str_data, root_name_off)
    root_path = "" if root_name == "/" else root_name

    consumed = parse_dir_content(6, root_path, root_nfiles, root_nsubs, root_sector)

    if consumed != len(u32s):
        print(f"Warning: consumed {consumed} u32s but entry table has {len(u32s)}", file=sys.stderr)

    return all_files


def extract_file(f, sector, size, output_path):
    """Extract a single file from the PAK to disk."""
    os.makedirs(os.path.dirname(output_path) if os.path.dirname(output_path) else '.', exist_ok=True)
    f.seek(sector * SECTOR_SIZE)
    data = f.read(size)
    with open(output_path, 'wb') as out:
        out.write(data)
    return len(data)


def main():
    parser = argparse.ArgumentParser(
        description='Extract files from Days of Thunder PSP DATA.PAK archives.'
    )
    parser.add_argument('pak_file', help='Path to DATA.PAK file')
    parser.add_argument('-o', '--output', default='.', help='Output directory (default: current dir)')
    parser.add_argument('-l', '--list', action='store_true', help='List files only, do not extract')
    parser.add_argument(
        '-f', '--filter', nargs='*',
        help='Extract only files whose path contains one of these substrings (case-insensitive)'
    )
    args = parser.parse_args()

    with open(args.pak_file, 'rb') as f:
        pak_size = f.seek(0, 2)
        f.seek(0)

        header = read_header(f)
        print(f"PAK Header:")
        print(f"  Table total size: {header['table_total_size']} (0x{header['table_total_size']:x})")
        print(f"  Entries size:     {header['entries_size']} (0x{header['entries_size']:x})")
        print(f"  Version:          {header['version']}")
        print(f"  Table offset:     {header['table_offset']} (0x{header['table_offset']:x})")
        print(f"  PAK file size:    {pak_size:,d} bytes ({pak_size / 1024 / 1024:.1f} MB)")
        print()

        entry_data, str_data = load_tables(f, header)
        files = parse_pak(entry_data, str_data)

        total_size = sum(size for _, _, size in files)
        print(f"Found {len(files)} files ({total_size:,d} bytes total):")
        print()
        print(f"  {'Path':<70s} {'Sector':>8s} {'Size':>12s}")
        print(f"  {'-'*70} {'-'*8} {'-'*12}")

        for path, sector, size in files:
            print(f"  {path:<70s} {sector:>8d} {size:>12,d}")

        print(f"  {'-'*70} {'-'*8} {'-'*12}")
        print(f"  {'Total':<70s} {'':>8s} {total_size:>12,d}")

        if args.list:
            return

        # Determine which files to extract
        extract_list = files
        if args.filter:
            extract_list = []
            for path, sector, size in files:
                for pattern in args.filter:
                    if pattern.lower() in path.lower():
                        extract_list.append((path, sector, size))
                        break

        if not extract_list:
            print("\nNo files matched the filter.")
            return

        print(f"\nExtracting {len(extract_list)} file(s) to {os.path.abspath(args.output)}/")
        for path, sector, size in extract_list:
            output_path = os.path.join(args.output, path.lstrip('/'))
            extracted = extract_file(f, sector, size, output_path)
            print(f"  {path} ({extracted:,d} bytes)")

        print(f"\nDone. Extracted {len(extract_list)} files.")


if __name__ == '__main__':
    main()
