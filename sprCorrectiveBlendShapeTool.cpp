
//
// Revision 1.1.1.1  2011/10/12 00:00:07  pouya
//
//
//
//  Using Scripts:
//


global proc sprCorrectiveBlendShapeTool_lazyBlendShape (string $parameter, string $bottomNodeResult, string $meSh, string $pose,string $bS, int $mirror)

{

float $poseValue;
  if (`optionMenu -q -v  floatorBooleanmenu`=="float")
     {
     $poseValue=`floatSliderGrp-q   -value  poseFloatValueField`;
     }    
     else
     {
     $poseValue=`floatField  -q  -value poseBooleanField`;
     }

//get the three digit value of the pose for namin convention purpose
float  $percentPoseValue=$poseValue * 100;
string $poseValueForNaming;
if ($percentPoseValue>0 && $percentPoseValue<100)
   {
   string $stringtheValue=$percentPoseValue;
   $poseValueForNaming=( "0" + $stringtheValue);
   }
   else if ($percentPoseValue==100 )
   {   
   $poseValueForNaming="100";
   }
   else if ($percentPoseValue==0 )
   {
   $poseValueForNaming="000";
   }    

// end getting values
	
	//for normal blendShape
	if ($meSh != "")
	{
    	if($mirror == 0)
    	{
    		//Create a Target geometry
    		//duplicate the posed shape as it is (posed), rename it and place it in target folder
    		string $duplicate[]=`duplicate -rr $meSh`;
    		string $targeT=`rename $duplicate[0] ($meSh + $pose + "_" + $poseValueForNaming + "_ModiObj")`;
    
    		parent $targeT modifiedObjectsGP ;
    		setAttr ($targeT + ".visibility") 0;
    
    		//Create a BlendShape Geometry
    		//duplicate original shape of the posed shape, rename it and place it in target folder
    		string $dummyArray[]={$meSh};
    		string $bsDup[]=`sprDuplicateOrigShape $dummyArray {"connectType", "0", "prefix", "", "suffix", ($pose + "_Trg"), "capitalize", "0", "verbose", "0"}`;
    		string $hello[]=`listRelatives -parent $bsDup[0]`;
    		//string $bS=`rename $hello[0] ($meSh + $pose + "_" + $poseValueForNaming + "_Trg")`;
    		rename $hello[0] $bS;
    		parent $bS targetGP ;
    		setAttr ($bS + ".visibility") 0;
    	}
    	//for mirror blendShape
    	else if ($mirror == 1)
    	{
    		//Create a Target geometry
    		//duplicate the posed shape as it is (posed), rename it and place it in target folder
    		//string $duplicate[]=`duplicate -rr $meSh`;
    		string $duplicate[]=`duplicate -rr ($meSh + $pose + "_" + $poseValueForNaming + "_Trg")`;
    		string $targeT=`rename $duplicate[0] ($meSh + $pose + "_" + $poseValueForNaming + "_ModiObj")`;
    		parent $targeT modifiedObjectsGP ;
    		setAttr ($targeT + ".visibility") 0;
    
    
    		setAttr ($bS + ".visibility") 0;
    	}
    	
    	
    	
    	//Blendshape 
    
    	//our desired blendshape name
    	string $bsName=($bottomNodeResult + "BS");
    
    	//list the blendshapes in the scene to check if a bs with our name already exists
    	string $blendShapes[]=`ls -type blendShape`;
    	int $count = stringArrayCount($bsName, $blendShapes);
    
    	if ($count==1)
    	   {
    	   //if it exist, add the new pose to our existing BS
    	   
    	   string $connections[] = `listConnections -type mesh $bsName`;
    	   int $bsNumChildren= `size($connections)`;
    	   
    	   
    	   int $index;
    	   if (attributeExists ("index", $bsName))
    		{
    			setAttr ($bsName + ".index") (`getAttr ($bsName + ".index")` + 1);
    			$index = `getAttr ($bsName + ".index")`;
    		}
    		else
    		{
    			//5 is just for safety, it is possible that we might have deleted some blendshapes
    		   //if add 5 digits to the index to make sure index numbers don't clash.
    		   //if by any change (%0.0001) it does clash!!  then increase this number to 10 or more
    		   $index = $bsNumChildren + 5 ;
    		   print ("\n" + $index + "\n");
    		   print("\n" + $bsName + "\n");
    			addAttr -ln "index"  -keyable true -at long -dv $index  $bsName;
    			setAttr ($bsName + ".index") $index;
    
    		}
    
    	   blendShape -e  -t $bottomNodeResult $index $bS 1 $bsName;
    	   setDrivenKeyframe -currentDriver ($parameter + "." + $pose) -driverValue 1 -value $poseValue ($bsName + "." + $bS) ;
    	   setDrivenKeyframe -currentDriver ($parameter + "." + $pose) -driverValue 0 -value 0 ($bsName + "." + $bS) ;
    
    	   } 
    	   else
    	   {
    	   //If not exist create a new one
    		select -r $bS $bottomNodeResult;
    		blendShape -frontOfChain -n $bsName;
    		addAttr -ln "index"  -keyable true -at long  -dv 0 $bsName;
    	   setDrivenKeyframe -currentDriver ($parameter + "." + $pose) -driverValue 1 -value $poseValue ($bsName + "." + $bS) ;
    	   setDrivenKeyframe -currentDriver ($parameter + "." + $pose) -driverValue 0 -value 0 ($bsName + "." + $bS) ;
    	   }
	   }
	   else
	   {
	       warning "It is possible that the UI is using a default name that does not exist in your scene. please update the UI";
	   }

}


proc creatModifiedObjectGroup()
{
    string $moObjs[]=`ls "modifiedObjectsGP"`;
    if (`size $moObjs`==0)   
       {
       group -em -name modifiedObjectsGP -parent setupGP;
       }
}




global proc sprCorrectiveBlendShapeTool_hidecage(){

  string $meSh = `sprTopMeshName`;
  string $pose=`optionMenu -q -v poseNameOptionMenu`;
  string $poseValue = sprFindPoseValue();
  string $targetObjs[]=`ls ($meSh + $pose + "_" + $poseValue + "_ModiObj")`;
  string $switcH=`getAttr ($targetObjs[0] + ".visibility")`;
  string $select=`textField  -q  -text  hideField`;

    if ($switcH==0)
       {
          setAttr ($select + ".visibility") 0;
          setAttr ($targetObjs[0] + ".visibility") 1;
        }
    else
       {
          setAttr ($select + ".visibility") 1;
          setAttr ($targetObjs[0] + ".visibility") 0;
       }
}


global proc  sprCorrectiveBlendShapeTool_fitToTarget()
  {
	string $meSh = `sprTopMeshName`;
    string $pose=`optionMenu -q -v poseNameOptionMenu`;
    string $poseValue = sprFindPoseValue();
    string $targetObjs[]=`ls ($meSh + $pose + "_" + $poseValue + "_ModiObj")`;
    string $BSObjs[]=`ls ($meSh + $pose + "_" + $poseValue + "_Trg")`;
    sprFitToTarget($meSh, $targetObjs[0], $BSObjs[0]);
  }
  
global proc sprCorrectiveBlendShapeTool_deleteTargets()
  {
  delete modifiedObjectsGP;
  }




proc string[] reverseArray( string $array[] ){
string $reversedArray[];
int $arraySize = `size $array`;

for( $n = 0; $n < $arraySize; $n++ ) $reversedArray[( $arraySize - 1 -
$n )] = $array[$n];
return $reversedArray;
}



//after finding the lowest bind set, i checks if the intended mesh exists in that set
//if not it goes to the next bottom set.
proc string lowestBindCage()
{
    string $bindSets[]=`listConnections  -d 0 -s 1 faceSetup_bindSet`;
	string $revBindSets[]=`reverseArray $bindSets`;
    for ($bindSet in $revBindSets)
    {   
        //find the clean setName
        string $startPartDef=startString($bindSet, 15);
        string $endPartDef=endString($bindSet, 3);
        string $withoutSetDef=substituteAllString($bindSet, $startPartDef, "");
        string $cleanSetName=substituteAllString($withoutSetDef, $endPartDef, "");
        
        $lowestCage = $cleanSetName + `sprNoPrefixMeshName`;
        if (`objExists $lowestCage`)
        {
           return  $lowestCage;
           break;
        }
        else
        {
            warning "Are you sure you are not using the default name in the UI? you must select a mesh in your setup and click on the plus sign to update the UI";
            return ("");
        }
        
    
    }
}



global proc string sprFindPoseValue()
{
	float $poseValue;
	  if (`optionMenu -q -v  floatorBooleanmenu`=="float")
		 {
		 $poseValue=`floatSliderGrp-q   -value  poseFloatValueField`;
		 }    
		 else
		 {
		 $poseValue=`floatField  -q  -value poseBooleanField`;
		 }

	//get the three digit value of the pose for namin convention purpose
	float  $percentPoseValue=$poseValue * 100;
	string $poseValueForNaming;
	if ($percentPoseValue>0 && $percentPoseValue<100)
	   {
	   string $stringtheValue=$percentPoseValue;
	   $poseValueForNaming=( "0" + $stringtheValue);
	   }
	   else if ($percentPoseValue==100 )
	   {   
	   $poseValueForNaming="100";
	   }
	   else if ($percentPoseValue==0 )
	   {
	   $poseValueForNaming="000";
	   }
	   return $poseValueForNaming;
}




proc createPosemenu()
{
        if (`optionMenu -exists poseNameOptionMenu`) deleteUI -menu poseNameOptionMenu;
        setParent parameterLayout;
        optionMenu -width 240 -changeCommand "sprCorrectiveBlendShapeTool_createField" poseNameOptionMenu;
        string $selectedParameter=`optionMenu -q -value parametersOptionMenu`;
        string $menuposes[]=`listAttr -keyable $selectedParameter`;
        for ($menupose in $menuposes)
            {
            menuItem -label $menupose;  
            }
}  



global proc sprCorrectiveBlendShapeTool_createField()
  {
     if (`floatSliderGrp -exists poseFloatValueField`) deleteUI -control poseFloatValueField;
     if (`floatField -exists poseBooleanField`) deleteUI -control poseBooleanField;

  //get the values
  string $parameter=`optionMenu -q  -value parametersOptionMenu`;
  string $pose=`optionMenu -q  -value poseNameOptionMenu`;
  float $values=`getAttr ($parameter + "." + $pose)`;
  //end get values
  
  if (`optionMenu -q -v  floatorBooleanmenu`=="float")
     {
     setParent setPoseValueLayout;
     floatSliderGrp  -field true -height 20 -columnWidth2 40 190  -minValue 0 -maxValue 1  -precision 2 -sliderStep 0.05 -value $values poseFloatValueField;
     connectControl poseFloatValueField ($parameter + "." + $pose);
     }
     else
     {
     setParent setPoseValueLayout;
     floatField  -height 20 -w 230 -minValue 0 -maxValue 1  -value $values -precision 0 poseBooleanField;
     connectControl poseBooleanField ($parameter + "." + $pose);
     }
  }   
  

global proc sprCorrectiveBlendShapeTool_getSelected(string $fieldName)
{
string $selected[]=`ls -sl`;
textField  -e -text $selected[0] $fieldName; 
}


global proc sprCorrectiveBlendShapeTool_getSelectedObject()
{
string $selected[]=`ls -sl`;
textField  -e -text $selected[0] objField; 
textField  -e -text $selected[0] hideField; 

}


global proc sprCorrectiveBlendShapeTool_changeComandParamMenu()
{
createPosemenu;
sprCorrectiveBlendShapeTool_createField;
select `optionMenu -q  -v  parametersOptionMenu`;
}
   
   
global proc sprDoBlendShape()
{
	string $parameter=`optionMenu -q  -value parametersOptionMenu`;
	string $meSh= `sprTopMeshName`;
	string $pose=`optionMenu -q  -value poseNameOptionMenu`;
	string $select=`textField  -q  -text  hideField`;
	string $bottomNodeResult=`lowestBindCage`;

	float $poseValue;
	if (`optionMenu -q -v  floatorBooleanmenu`=="float")
     {
     $poseValue=`floatSliderGrp-q   -value  poseFloatValueField`;
     }    
     else
     {
     $poseValue=`floatField  -q  -value poseBooleanField`;
     }
     
	//check if the pose value is set to 1.00
	//if not give warning
	if ($poseValue!=1)
	{
		warning "pose value is not set to 1, blendshape created for a value other than 1";
	}
	//get the three digit value of the pose for namin convention purpose
	float  $percentPoseValue=$poseValue * 100;
	string $poseValueForNaming;
	if ($percentPoseValue>0 && $percentPoseValue<100)
	{
		string $stringtheValue=$percentPoseValue;
		$poseValueForNaming=( "0" + $stringtheValue);
	}
	else if ($percentPoseValue==100 )
	{   
		$poseValueForNaming="100";
	}
	else if ($percentPoseValue==0 )
	{
		$poseValueForNaming="000";
	} 
   
	string $bs = $meSh + $pose + "_" + $poseValueForNaming + "_Trg";
	if(`objExists $bs`)
	{
		warning "there is already a blendShape setup for this pose";
	}
	else
	{
		sprCorrectiveBlendShapeTool_lazyBlendShape ( $parameter, $bottomNodeResult,$meSh,$pose,$bs,0);
	}

}      
  
global proc sprDoMirrorBlendShape()
{
	string $parameter=`optionMenu -q  -value parametersOptionMenu`;
	string $meSh = `sprTopMeshName`;
	string $pose=`optionMenu -q  -value poseNameOptionMenu`;
	string $select=`textField  -q  -text  hideField`;
	string $bottomNodeResult=`lowestBindCage`;


	//get the pose value
	float $poseValue;
	if (`optionMenu -q -v  floatorBooleanmenu`=="float")
     {
		$poseValue=`floatSliderGrp-q   -value  poseFloatValueField`;
     }    
     else
     {
		$poseValue=`floatField  -q  -value poseBooleanField`;
     }

	//get the three digit value of the pose for namin convention purpose
	float  $percentPoseValue=$poseValue * 100;
	string $poseValueForNaming;
	if ($percentPoseValue>0 && $percentPoseValue<100)
	   {
	   string $stringtheValue=$percentPoseValue;
	   $poseValueForNaming=( "0" + $stringtheValue);
	   }
	   else if ($percentPoseValue==100 )
	   {   
	   $poseValueForNaming="100";
	   }
	   else if ($percentPoseValue==0 )
	   {
	   $poseValueForNaming="000";
	   }   
	//get the pose value end


	//get the mirror side pose name
	string $mirrorPose;
	if (endString($pose, 1) == "L" || endString($pose, 1) == "R")
	{
		string $lr = endString($pose, 1);
		if ($lr == "L")
		{
			$mirrorPose= substituteAllString($pose, $lr, "R");
		}
		else if ($lr == "R")
		{
			$mirrorPose= substituteAllString($pose, $lr, "L");
		}

		//check if there is a blendshape for the current side
		string $bs = $meSh + $pose + "_" + $poseValueForNaming + "_Trg";
		
		string $mirrorBs = $meSh + $mirrorPose + "_" + $poseValueForNaming + "_Trg";

		if (`objExists $bs` != 1)
		{
			warning "Target object not found, There is no blendShape setup for this pose to be mirrored";
		}
		//check if there is a blendshape for the oppositeside
		else if(`objExists $mirrorBs` == 1)
		{
			warning "There is already a blendShape setup for the opposite side, please delete that one to be able to mirror this.";
		}
		else
		{
			//if the cage has a mirror (such as eyebrowCageL,eyebrowCageR)
			//get the mirror name
			string $mirrorMesh;
			if (endString($meSh, 1) == "L" || endString($meSh, 1) == "R")
			{
				string $lr = endString($meSh, 1);
				if ($lr == "L")
				{
					$mirrorMesh = substituteAllString($meSh, $lr, "R");
				}
				else if ($lr == "R")
				{
					$mirrorMesh = substituteAllString($meSh, $lr, "L");
				}	
			}
			else
			{
				$mirrorMesh = $meSh;
			}

			//duplicate the target mesh and rename it for the opposite side
			string $bsMirror = $mirrorMesh + $mirrorPose + "_" + $poseValueForNaming + "_Trg";
			//duplicate -name $bsMirror  $bs;

			string $dupOrig[] = `sprDuplicateOrigShape {$meSh} {"connectType", "0", "prefix", "", "suffix", "_orig", "capitalize", "0", "verbose", "0"}`;

			string $origMesh[] =`listRelatives -parent  $dupOrig[0]`;

			select -r $origMesh[0];
			select -add $bs;

			string $swapMesh[] = `sprCreateMirrorGeometry "" "" "swap" "YZ" 1 {}`;
			rename $swapMesh[0] $bsMirror;
			parent $bsMirror "targetGP";
			delete $origMesh[0];


			//setup the blendshape
			sprCorrectiveBlendShapeTool_lazyBlendShape ( $parameter, $bottomNodeResult,$mirrorMesh,$mirrorPose,$bsMirror,1);

				  
			//check if the pose value is set to 1.00
			//if not give warning
			if ($poseValue!=1)
			   {
			   warning "pose value is not set to 1, blendshape created for a value other than 1";
			   }
		}

	}
	else
	{
		warning "This pose can not be mirrored";
	}


}



//this procedure gets a name from the Ui and returns the mesh name without
//set prefix  or if the mesh  doesnt have a prefix, it will capitalize the first letter
//and return it
global proc string sprNoPrefixMeshName()
{
    string $bindSets[]=`sets -q  faceSetup_bindSet`;
    for ($bindSet in $bindSets)
    {
        
        //find the clean setName
        string $startPartDef=startString($bindSet, 15);
        string $endPartDef=endString($bindSet, 3);
        string $withoutSetDef=substituteAllString($bindSet, $startPartDef, "");
        string $cleanSetName=substituteAllString($withoutSetDef, $endPartDef, "");
        
        //find the clean cage name
        string $meSh=`textField  -q  -text  objField`;
        string $currentSet = startString($meSh, `size $cleanSetName`);
        string $cleanMeshName;
        
        if(isParentOf("cageGP", $meSh))
        {
            $cleanMeshName = capitalizeString($meSh);
            return $cleanMeshName;
        }
        else if($currentSet  == $cleanSetName)
        {      
            $cleanMeshName = substituteAllString($meSh, $currentSet, "");
            return $cleanMeshName;
            
        }
    
    }  
}


//this procedure returns the top level mesh
//from the "object to be deformed" field.
//the selected mesh might not be the top level
//but this procedure automatically finds the top level
//node of the same mesh in the hierarchy and returns it
global proc string sprTopMeshName()
{
    string $bindSets[]=`listConnections  -d 0 -s 1 faceSetup_bindSet`;
    for ($bindSet in $bindSets)
    {   
        //find the clean setName
        string $startPartDef=startString($bindSet, 15);
        string $endPartDef=endString($bindSet, 3);
        string $withoutSetDef=substituteAllString($bindSet, $startPartDef, "");
        string $cleanSetName=substituteAllString($withoutSetDef, $endPartDef, "");
        
        $topMesh = $cleanSetName + `sprNoPrefixMeshName`;
        if (`objExists $topMesh`)
        {
           return  $topMesh;
           break;
        }
        else
        {
            warning "Are you sure you have selected a mesh in your facesetup and not just the default Ui value?";
            return ("");
        }
    
    }
}


global proc sprDeleteBlendShape ()
{
    string $parameter=`optionMenu -q  -value parametersOptionMenu`;
    string $meSh= `sprTopMeshName`;
    string $pose=`optionMenu -q  -value poseNameOptionMenu`;
    string $bs = $meSh + $pose + "_*_Trg";
    string $bsName[];
    if (`objExists $bs`)
    {
        $bsName = `listConnections  -d 1 -s 0 ($meSh + $pose + "_*_TrgShape")`;
    
        //list the blendshapes in the scene to check if a bs with our name already exists
        string $blendShapes[]=`ls -type blendShape`;
        int $count = stringArrayCount($bsName[0], $blendShapes);
        
        if ($count)
           { 
    			string $targets[] = `blendShape -q  -target $bsName`;
    			int $i = 1;
    			int $targetIndex;
    			for ($target in $targets)
    			{
    				if ($target == $bs)
    				{
    					$targetIndex = $i;
    				}
    				$i++;
    			}
    			blendShape -e  -tc 0 -remove -target $meSh $targetIndex $bs 1 -t $meSh $targetIndex $meSh 1 $bsName;
    			string $modiObj = $meSh + $pose + "_*_ModiObj";
    			if (`objExists $modiObj`)
    			{
    				delete $modiObj;
    				delete $bs;
    			}    
           } 
           else
           {
        	   warning "There is no blendShape setup for this pose to delete";
           }
    }
    else
    {
        warning "Target mesh is missing!";  
    }

}





proc createWindow()
{ 

//get all the values needed to run the script
      
      
      
if (`window -exists sprLazyBlendShapeTool`) 
    {
    deleteUI -window sprLazyBlendShapeTool;
    windowPref -remove sprLazyBlendShapeTool;
    }
window -title "sprLazyBlendShapeTool" -resizeToFitChildren 1 -width 252 sprLazyBlendShapeTool; 
columnLayout  mainColumn;
columnLayout -columnAttach "both" 5 -rowSpacing 5 -columnAlign "center" -columnWidth 250 parameterLayout;
separator -style "none";


text -label "object to be deformed:" -align "left";
rowLayout -numberOfColumns 2   toBeDeformedLayout;
button  -label "+" -height 25 -w 25 -command "sprCorrectiveBlendShapeTool_getSelectedObject" toBeDeformedButton;
textField  -height 25 -w 210 -text "faceHeadCage" objField;
setParent ..;
separator -style "in";
text -label "Parameter name:" -align "left";
        optionMenu -changeCommand "sprCorrectiveBlendShapeTool_changeComandParamMenu" -width 240 parametersOptionMenu;
        string $parameters[]=`sets -q  faceSetup_parameterSet`;
        for ($parameter in $parameters)
            {
             menuItem -label $parameter ;   
            } 


separator -style "in";
text -label "Pose Name:" -align "left";
createPosemenu;

setParent ..;
columnLayout -columnAttach "both" 5 -rowSpacing 5 -columnAlign "center" -columnWidth 250 buttonsLayout;

separator -style "none";
separator -style "in";
text -label "Set pose value:" -align "left";
rowLayout -numberOfColumns 4     -columnAlign  1 "right"     -columnAttach 2 "both"  0  -columnAttach 3 "both"  10 setPoseValueLayout;


setParent ..;


separator -style "in";  
button  -label "Creat BS and connect to Parameter" -command "sprDoBlendShape" -height 40 blendShapeButton;

rowLayout -numberOfColumns 2   toBeDeformedLayout;
button  -label "mirror BlendShape" -command "sprDoMirrorBlendShape" -height 25 -w 118 blendShapeMirrorButton;
button  -label "delete blendShape" -command "sprDeleteBlendShape" -height 25 -w 118 deleteBlendShapeButton;
setParent ..;
separator -style "in";


button  -label "Creat Difference" -height 40 -command "sprCorrectiveBlendShapeTool_fitToTarget";


separator -style "in";

frameLayout -label "Utilities"
               -borderStyle "in" -collapsable 1 -collapse 1;
                columnLayout;
                    text -label " select the cages that you wish to swtich " -align "left";
text -label " visibilty on when working on target cage:" -align "left";
separator -style "none";

rowLayout -numberOfColumns 3   switchVisLayout;
button  -label "+" -height 25 -w 25 -command "sprCorrectiveBlendShapeTool_getSelected hideField" loadSwitchButton;

textField  -height 25  -w 120 -text "faceHeadCage" hideField;
button  -label "switch" -height 25 -w 85 -command "sprCorrectiveBlendShapeTool_hidecage";
                    setParent ..;
separator -style "in"; 
text -label "" -align "left";


button  -label "delete Targets" -height 25  -w 235 -c "sprCorrectiveBlendShapeTool_deleteTargets";
 

separator -style "in"; 
text -label "" -align "left";
rowLayout -numberOfColumns 2   poseValueLayout;
text -label " Change pose value type to: " -align "left";

optionMenu -changeCommand "sprCorrectiveBlendShapeTool_createField" -width 90 floatorBooleanmenu;
   menuItem -label "float";
   menuItem -label "boolean";
sprCorrectiveBlendShapeTool_createField;



setParent ..;
separator -style "none"; 
setParent ..;
separator -style "none"; 
separator -style "none"; 
showWindow sprLazyBlendShapeTool;

string $parameter=`optionMenu -q  -value parametersOptionMenu`;
string $meSh=`textField  -q  -text  objField`;
string $pose=`optionMenu -q  -value poseNameOptionMenu`;


}



global proc sprCorrectiveBlendShapeTool(string $options[])
{
createWindow;
creatModifiedObjectGroup;
string $parameter=`optionMenu -q  -value parametersOptionMenu`;
string $meSh=`textField  -q  -text  objField`;
string $pose=`optionMenu -q  -value poseNameOptionMenu`;
string $select=`textField  -q  -text  hideField`;
}
