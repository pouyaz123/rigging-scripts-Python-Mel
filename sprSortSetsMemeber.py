import maya.cmds as cmds
from re import match
      
#sprSortSetsMemeber("addAttrList")
#python("sprSortSetsMemeber(\"addAttrList\")");      
##########        
            
import maya.cmds as cmds
     
##########        
            
def curveControllers(set):
    controllers = cmds.sets(set,q=1)
    allCurveShapes = cmds.ls(type='nurbsCurve')
    
    curveControllers = []
    for curveShape in allCurveShapes:
        curveNode = cmds.listRelatives(curveShape, parent=True)
        if curveNode[0] in controllers:
            if curveNode[0] not in curveControllers:
                curveControllers.append(curveNode[0])
           
    
    curveControllers.sort()

    return (curveControllers)



def jointControllers(set):
    controllers = cmds.sets(set,q=1)
    jointControllers = cmds.ls(controllers,exactType='joint')
    jointControllers.sort()

    return (jointControllers)

    
def transformControllers(set,defaultControlers):
    controllers = cmds.sets(set,q=1)
    allTransforms = cmds.ls(controllers,exactType='transform')
    
    
    
    transformControllers = []
    curveController = curveControllers(set)


    jointController = jointControllers(set)
    for node in allTransforms:
        if node not in curveController:
            if node not in jointController:
                if node not in defaultControlers:
                    transformControllers.append(node)
                
    transformControllers.sort()
    sortedTransforms = []
    for controller in defaultControlers:
        if controller in controllers :
            sortedTransforms.append(controller)
            
    sortedTransforms.extend(transformControllers)

    return (sortedTransforms)
    
    
    
    
    
def sprSortSetsMemeber(set, sortByType = True, defaultControlers = ["browController","eyeController","noseController","mouthController"]):
    
    allControllers = cmds.sets(set,q=1)
    
    sortedControllerList = []
    if sortByType == True:
        sortedControllerList.extend(transformControllers(set,defaultControlers))
        sortedControllerList.extend(curveControllers(set))
        sortedControllerList.extend(jointControllers(set))
    else:
        for defaultControler in defaultControlers:
            if defaultControler in allControllers:
                allControllers.remove(defaultControler)
            else:
                defaultControlers.remove(defaultControler)
                
                
        allControllers.sort()
        sortedControllerList.extend(defaultControlers)
        sortedControllerList.extend(allControllers)
        

    for item in sortedControllerList:      
        cmds.sets(item, remove=set)

         
    for item in sortedControllerList:
        cmds.sets(item, addElement=set)