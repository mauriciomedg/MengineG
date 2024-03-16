import bpy
import ctypes

testlib = ctypes.CDLL("C:\\MengineG\\x64\\Debug\\BlenderPlugin.dll");

#Blender Python scripting: Creating custom operators from scratch

#class <CATEGORY>_<TYPE>_<name>
class WM_OT_RunGameEngine(bpy.types.Operator):
    bl_idname = "wm.run_game_engine"
    bl_label = "Start MGameEngine"
    
    def execute(self, context):
        spam = testlib.getInstance()
        testlib.execute(spam)

        print("Game Engine Start")
        return {'FINISHED'}
    
bpy.utils.register_class(WM_OT_RunGameEngine)
bpy.utils.unregister_class(WM_OT_RunGameEngine)

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
        
        
def register():
    bpy.utils.register_class(WM_OT_RunGameEngine)
    bpy.utils.register_class(MGAMEENGINE_PT_main_panel)

def unregister():
    bpy.utils.unregister_class(WM_OT_RunGameEngine)
    bpy.utils.register_class(MGAMEENGINE_PT_main_panel)
    
if __name__ == "__main__":
    register()
