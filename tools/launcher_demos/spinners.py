"""3D-wireframe-to-ASCII rotator using braille subpixel rendering.

Each terminal character is a 2×4 braille dot grid (Unicode U+2800..U+28FF),
giving ~8× the effective resolution vs plain block chars. Lines rasterize
via Bresenham in dot-space for smooth rotation with minimal character-jump.

Shapes:
  tesseract        — 4D hypercube (inner + outer cubes with pulsing w-scale)
  sphere           — lat/long wireframe (very smooth rotation)
  torus            — donut wireframe
  icosphere        — subdivided icosahedron (spikey-ball)
"""

import math


# ─── Braille canvas ─────────────────────────────────────────────────────────

# Per Unicode standard:
#   0  3
#   1  4
#   2  5
#   6  7
_DOT_BIT = {
    (0, 0): 0x01,  (1, 0): 0x08,
    (0, 1): 0x02,  (1, 1): 0x10,
    (0, 2): 0x04,  (1, 2): 0x20,
    (0, 3): 0x40,  (1, 3): 0x80,
}


class BrailleCanvas:
    def __init__(self, char_w, char_h):
        self.cw = char_w
        self.ch = char_h
        self.dw = 2 * char_w
        self.dh = 4 * char_h
        self.dots = [[0] * self.dw for _ in range(self.dh)]

    def set(self, x, y):
        ix, iy = int(x), int(y)
        if 0 <= ix < self.dw and 0 <= iy < self.dh:
            self.dots[iy][ix] = 1

    def line(self, x0, y0, x1, y1):
        """Bresenham's line in dot-space."""
        x0, y0, x1, y1 = int(round(x0)), int(round(y0)), int(round(x1)), int(round(y1))
        dx = abs(x1 - x0)
        dy = abs(y1 - y0)
        sx = 1 if x0 < x1 else -1
        sy = 1 if y0 < y1 else -1
        err = dx - dy
        while True:
            self.set(x0, y0)
            if x0 == x1 and y0 == y1:
                break
            e2 = 2 * err
            if e2 > -dy:
                err -= dy
                x0 += sx
            if e2 < dx:
                err += dx
                y0 += sy

    def render(self):
        out = []
        for cy in range(self.ch):
            row = []
            for cx in range(self.cw):
                code = 0x2800
                for dx in range(2):
                    for dy in range(4):
                        if self.dots[cy * 4 + dy][cx * 2 + dx]:
                            code |= _DOT_BIT[(dx, dy)]
                row.append(chr(code))
            out.append("".join(row))
        return out


# ─── shapes: (vertices, edges) ──────────────────────────────────────────────

def _cube_verts_edges():
    verts = [(x, y, z) for x in (-1, 1) for y in (-1, 1) for z in (-1, 1)]
    edges = [(i, j) for i in range(8) for j in range(i + 1, 8)
             if sum(1 for k in range(3) if verts[i][k] != verts[j][k]) == 1]
    return verts, edges


def tesseract():
    cube_v, cube_e = _cube_verts_edges()
    inner = [(0.5 * x, 0.5 * y, 0.5 * z) for x, y, z in cube_v]
    verts = list(cube_v) + inner
    edges = list(cube_e)
    edges += [(a + 8, b + 8) for a, b in cube_e]
    edges += [(i, i + 8) for i in range(8)]
    return verts, edges


def sphere(n_lat=10, n_lng=16):
    """Sphere wireframe with lat/long grid. n_lat latitude bands, n_lng longitudes."""
    verts = []
    for i in range(n_lat + 1):
        theta = math.pi * i / n_lat
        for j in range(n_lng):
            phi = 2 * math.pi * j / n_lng
            x = math.sin(theta) * math.cos(phi)
            y = math.cos(theta)
            z = math.sin(theta) * math.sin(phi)
            verts.append((x, y, z))
    edges = []
    for i in range(n_lat + 1):
        for j in range(n_lng):
            a = i * n_lng + j
            b = i * n_lng + (j + 1) % n_lng
            if 0 < i < n_lat:
                edges.append((a, b))
    for i in range(n_lat):
        for j in range(n_lng):
            a = i * n_lng + j
            b = (i + 1) * n_lng + j
            edges.append((a, b))
    return verts, edges


def torus(R=0.7, r=0.3, n_u=20, n_v=10):
    """Torus: major radius R, minor radius r. n_u around major, n_v around minor."""
    verts = []
    for i in range(n_u):
        u = 2 * math.pi * i / n_u
        for j in range(n_v):
            v = 2 * math.pi * j / n_v
            x = (R + r * math.cos(v)) * math.cos(u)
            y = r * math.sin(v)
            z = (R + r * math.cos(v)) * math.sin(u)
            verts.append((x, y, z))
    edges = []
    for i in range(n_u):
        for j in range(n_v):
            a = i * n_v + j
            b = i * n_v + (j + 1) % n_v
            c = ((i + 1) % n_u) * n_v + j
            edges.append((a, b))
            edges.append((a, c))
    return verts, edges


def icosphere():
    """Icosahedron with each face subdivided once (80-triangle geodesic ball)."""
    phi = (1 + math.sqrt(5)) / 2
    s = 1 / math.sqrt(1 + phi * phi)
    base_verts = [
        ( 0,  s,  s*phi), ( 0,  s, -s*phi),
        ( 0, -s,  s*phi), ( 0, -s, -s*phi),
        ( s,  s*phi, 0), ( s, -s*phi, 0),
        (-s,  s*phi, 0), (-s, -s*phi, 0),
        ( s*phi, 0,  s), ( s*phi, 0, -s),
        (-s*phi, 0,  s), (-s*phi, 0, -s),
    ]
    # Icosahedron faces (indices)
    base_faces = [
        (0, 4, 6), (0, 6, 10), (0, 10, 2), (0, 2, 8), (0, 8, 4),
        (3, 1, 11), (3, 11, 7), (3, 7, 5), (3, 5, 9), (3, 9, 1),
        (4, 1, 6), (6, 1, 11), (6, 11, 10), (10, 11, 7), (10, 7, 2),
        (2, 7, 5), (2, 5, 8), (8, 5, 9), (8, 9, 4), (4, 9, 1),
    ]

    def norm(v):
        L = math.sqrt(sum(c * c for c in v))
        return tuple(c / L for c in v)

    def midpoint(v1, v2):
        return norm(tuple((v1[k] + v2[k]) / 2 for k in range(3)))

    verts = list(base_verts)
    mid_cache = {}

    def get_mid(i, j):
        key = (min(i, j), max(i, j))
        if key in mid_cache:
            return mid_cache[key]
        m = midpoint(verts[i], verts[j])
        verts.append(m)
        idx = len(verts) - 1
        mid_cache[key] = idx
        return idx

    # Subdivide each face into 4 triangles
    new_faces = []
    for a, b, c in base_faces:
        ab = get_mid(a, b)
        bc = get_mid(b, c)
        ca = get_mid(c, a)
        new_faces.extend([(a, ab, ca), (b, bc, ab), (c, ca, bc), (ab, bc, ca)])

    edge_set = set()
    for a, b, c in new_faces:
        for x, y in ((a, b), (b, c), (c, a)):
            edge_set.add((min(x, y), max(x, y)))
    return verts, list(edge_set)


SHAPES = {
    "tesseract":  tesseract,
    "sphere":     sphere,
    "torus":      torus,
    "icosphere":  icosphere,
}


# ─── rotation + projection ──────────────────────────────────────────────────

def rotate(vertices, ax, ay, az=0.0):
    cx, sx = math.cos(ax), math.sin(ax)
    cy, sy = math.cos(ay), math.sin(ay)
    cz, sz = math.cos(az), math.sin(az)
    out = []
    for x, y, z in vertices:
        y, z = y * cx - z * sx, y * sx + z * cx
        x, z = x * cy + z * sy, -x * sy + z * cy
        x, y = x * cz - y * sz, x * sz + y * cz
        out.append((x, y, z))
    return out


def render(shape_name, angle_x, angle_y, width=18, height=9, t=0.0):
    """Render the shape to braille characters. Returns list of strings."""
    verts, edges = SHAPES[shape_name]()

    if shape_name == "tesseract":
        pulse = 0.5 + 0.15 * math.sin(t * 1.2)
        verts = verts[:8] + [(x * pulse / 0.5, y * pulse / 0.5, z * pulse / 0.5)
                             for x, y, z in verts[8:]]

    verts = rotate(verts, angle_x, angle_y)

    canvas = BrailleCanvas(width, height)

    # Dot-space dims
    dw = 2 * width
    dh = 4 * height
    # Character cells are 2 dots wide × 4 dots tall, so to preserve aspect
    # ratio we scale y by 2 (4/2) less than x.
    cx_d = dw / 2
    cy_d = dh / 2
    sx_d = (dw - 2) / 3.0
    sy_d = (dh - 4) / 3.0

    for a, b in edges:
        xa, ya, _za = verts[a]
        xb, yb, _zb = verts[b]
        x0 = cx_d + xa * sx_d
        y0 = cy_d - ya * sy_d
        x1 = cx_d + xb * sx_d
        y1 = cy_d - yb * sy_d
        canvas.line(x0, y0, x1, y1)

    return canvas.render()
