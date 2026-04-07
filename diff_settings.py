def apply(config, args):
    config["arch"] = "mipsel"
    config["baseimg"] = "extern/iso_extract/PSP_GAME/SYSDIR/EBOOT.BIN.dec"
    config["myimg"] = "build/EBOOT.elf"
    config["mapfile"] = "build/EBOOT.map"
    config["build_dir"] = "build/"
    config["expected_dir"] = "expected/"
    config["source_directories"] = ["src/", "include/"]
    config["objdump_executable"] = "mipsel-linux-gnu-objdump"
    config["map_format"] = "gnu"
    config["makeflags"] = []
