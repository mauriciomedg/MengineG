import bpy
import ctypes
import os
import gc
import numpy as np
import mathutils

blend_dir = os.path.dirname(bpy.data.filepath)
lib_path = os.path.join(blend_dir, 'BlenderPlugin.dll') 
print("Load dll from: ", lib_path)
testlib = None #ctypes.CDLL(lib_path);

#testlib = ctypes.CDLL("D:\\CustomEngine\\MengineG\\x64\\Debug\\BlenderPlugin.dll");

#Blender Python scripting: Creating custom operators from scratch

#class <CATEGORY>_<TYPE>_<name>
class WM_OT_RunGameEngine(bpy.types.Operator):
    bl_idname = "wm.run_game_engine"
    bl_label = "Start MGameEngine"
    
    def execute(self, context):
        global testlib
        
        if testlib is None:
            testlib = ctypes.CDLL(lib_path)
            testlib.createEntity.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_float]
            
            testlib.createEntityMesh.argtypes = [
            ctypes.POINTER(ctypes.c_float), # vertices (float array)
            ctypes.c_int, # vertex_count (int)
            ctypes.POINTER(ctypes.c_int), # indices (int array)
            ctypes.c_int, # index_count (int)
            ctypes.POINTER(ctypes.c_float), # texture coordinates (float array)
            ctypes.c_int # coords_count (int)
            ]
            
            testlib.getInstance()
            testlib.start()
            print("BlenderPlugin.dll loaded")
        else:
            testlib.start()
            print("BlenderPlugin.dll already loaded")
            
        print("Game Engine Start")
        return {'FINISHED'}
    
bpy.utils.register_class(WM_OT_RunGameEngine)
bpy.utils.unregister_class(WM_OT_RunGameEngine)


class WM_OT_CreateEntity(bpy.types.Operator):
    bl_idname = "wm.create_entity"
    bl_label = "Create Entity"
    
    def defaultDataTest(self):
        obj = bpy.context.view_layer.objects.active
        loc = obj.location.copy()
        testlib.createEntity(loc[0], loc[2], loc[1])
        print("Create Entity getting the vertex data", obj.name)
        return
    
    def getVertexData(self):
        obj = bpy.context.view_layer.objects.active
        
        if obj.type == 'MESH':

            #Get transform matrix
            transform_matrix = obj.matrix_world
            
            conversion_matrix = mathutils.Matrix((
                (1, 0,  0, 0),   # X axis remains the same
                (0, 0,  1, 0),   # Y axis is mapped to Z
                (0, -1, 0, 0),   # Z axis is mapped to -Y
                (0, 0,  0, 1),
            ))
            
            blender_to_opengl = conversion_matrix @ transform_matrix

            transform_matrix_buffer = [blender_to_opengl[i][j] for i in range(4) for j in range(4)]
            transform_matrix_flat = np.array(transform_matrix_buffer, dtype = np.float32)
            
            mesh = obj.data
            # Update the mesh (important if you've done any operations that change the mesh structure)
            mesh.calc_loop_triangles()

            # Extract vertex coordinates as a flat list
            vertex_buffer_mesh = [coord for vertex in mesh.vertices for coord in vertex.co]
            vertex_buffer_flat = np.array(vertex_buffer_mesh, dtype = np.float32)
            
            # Extract face indices as a flat list
            # Extract index data (triangles)
            index_buffer_mesh = []
            for tri in mesh.loop_triangles:
                index_buffer_mesh.extend(tri.vertices)
    
            #index_buffer_mesh = [index for poly in mesh.polygons for index in poly.vertices]
            index_buffer_flat = np.array(index_buffer_mesh, dtype=np.int32)
            
            # Extract texture coordintates as a flat list
            texture_coords_mesh = []
            
            if mesh.uv_layers.active is not None:
                uv_layer = mesh.uv_layers.active.data
                # Extract UV coordinates as a flat list
                texture_coords_mesh = [uv.uv[i] for uv in uv_layer for i in range(2)]
             
            texture_coords_flat = np.array(texture_coords_mesh, dtype=np.float32)   
            
        
            # Convert all the numpys array to a ctypes pointer
            vertex_ptr = vertex_buffer_flat.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
            index_ptr = index_buffer_flat.ctypes.data_as(ctypes.POINTER(ctypes.c_int))
            texture_coords_ptr = texture_coords_flat.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
            transform_matrix_ptr = transform_matrix_flat.ctypes.data_as(ctypes.POINTER(ctypes.c_float))

            testlib.createEntityMesh(vertex_ptr, len(vertex_buffer_mesh), index_ptr, len(index_buffer_mesh), texture_coords_ptr, len(texture_coords_mesh), transform_matrix_ptr)
            
            print("Vertex Buffer:", len(vertex_buffer_mesh))
            print("Index Buffer:", len(index_buffer_mesh))
            print("Texture Buffer:", len(texture_coords_mesh))
        else:
            print("The active object is not a mesh.")
            
        print("Create Entity getting the vertex data", obj.name)
        return
    
    def execute(self, context):
        #obj = bpy.context.active_object
        global testlib
        
        if testlib is not None:
            self.getVertexData()
            #self.defaultDataTest()

        return {'FINISHED'}
    
bpy.utils.register_class(WM_OT_CreateEntity)
bpy.utils.unregister_class(WM_OT_CreateEntity)

class WM_OT_Quit(bpy.types.Operator):
    bl_idname = "wm.quit"
    bl_label = "Quit  Game"
    
    def execute(self, context):
        global testlib
        if testlib is not None:
            testlib.quit()
            del testlib
            testlib = None
            gc.collect()
            print("Quit Game and kill dll") 
            
        return {'FINISHED'}
    
bpy.utils.register_class(WM_OT_Quit)
bpy.utils.unregister_class(WM_OT_Quit)

# test call the operator
#bpy.ops.wm.run_game_engine()

class MGAMEENGINE_PT_main_panel(bpy.types.Panel) :
    bl_label = "MGameEngine"
    bl_idname = "MGAMEENGINE_PT_main_panel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'MGameEngine'

    def draw(self, context) :
        layout = self.layout
        
        #row = layout.row()
        #row.label(text = ".MG.", icon = 'FORCE_MAGNETIC')
        
        row = layout.row()
        row.operator("wm.run_game_engine", icon = 'FORCE_MAGNETIC')
        
        self.layout.separator()
        
        row = layout.row()
        row.operator("wm.create_entity", icon = 'CUBE')
        
        self.layout.separator()
        
        row = layout.row()
        row.operator("wm.quit", icon = 'CUBE')
        
def register():
    bpy.utils.register_class(WM_OT_RunGameEngine)
    bpy.utils.register_class(MGAMEENGINE_PT_main_panel)
    bpy.utils.register_class(WM_OT_CreateEntity)
    bpy.utils.register_class(WM_OT_Quit)

def unregister():
    bpy.utils.unregister_class(WM_OT_RunGameEngine)
    bpy.utils.unregister_class(MGAMEENGINE_PT_main_panel)
    bpy.utils.unregister_class(WM_OT_CreateEntity)
    bpy.utils.unregister_class(WM_OT_Quit)
    
if __name__ == "__main__":
    register()
