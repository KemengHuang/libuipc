import numpy as np
import polyscope as ps
import polyscope.imgui as psim
from pyuipc_loader import pyuipc
from pyuipc_loader import AssetDir
from pyuipc import Matrix4x4, Logger
from pyuipc.world import *
from pyuipc.engine import *
from pyuipc.constitution import *
from pyuipc.geometry import *

def process_surface(sc: SimplicialComplex):
    label_surface(sc)
    label_triangle_orient(sc)
    sc = flip_inward_triangles(sc)
    return sc

Logger.set_level(Logger.Level.Warn)

workspace = AssetDir.output_path(__file__)

engine = Engine("cuda", workspace)
world = World(engine)

config = Scene.default_config()
print(config)

scene = Scene(config)

snk = StableNeoHookean()
scene.constitution_tabular().insert(snk)
scene.contact_tabular().default_model(0.5, 1e9)
default_element = scene.contact_tabular().default_element()

pre_trans = pyuipc.Matrix4x4.Identity()

# scaling
pre_trans[0,0] = 0.2
pre_trans[1,1] = 0.2
pre_trans[2,2] = 0.2

io = SimplicialComplexIO(pre_trans)
cube = io.read(f'{AssetDir.tetmesh_path()}/cube.msh')
cube = process_surface(cube)

moduli = ElasticModuli.youngs_poisson(1e4, 0.49)
snk.apply_to(cube, moduli)
default_element.apply_to(cube)

object = scene.objects().create("object")
N = 10

trans = Matrix4x4.Identity()

for i in range(N):
    pos_v = view(cube.positions())
    for j in range(len(pos_v)):
        pos_v[j][1] += 0.24
    object.geometries().create(cube)

g = ground(0.0)
object.geometries().create(g)

sio = SceneIO(scene)
world.init(scene)

run = False

ps.init()
ps.set_ground_plane_mode('none')
s = sio.surface()
v = s.positions().view()
t = s.triangles().topo().view()
mesh = ps.register_surface_mesh('obj', v.reshape(-1,3), t.reshape(-1,3))
mesh.set_edge_width(1.0)
def on_update():
    global run
    if(psim.Button("run & stop")):
        run = not run
        
    if(run):
        world.advance()
        world.retrieve()
        s = sio.surface()
        v = s.positions().view()
        mesh.update_vertex_positions(v.reshape(-1,3))

ps.set_user_callback(on_update)
ps.show()