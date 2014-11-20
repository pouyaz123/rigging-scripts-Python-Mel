#!/usr/bin/python -t

#
#               Copyright (c) 2012 Sprite Entertainment, Inc.
#                             All Rights Reserved.
#
#  This file contains unpublished confidential and proprietary information
#  of Sprite Entertainment, Inc.  The contents  of  this  file  may not be
#  copied  or duplicated,  in whole  or  in part, by any means, electronic
#  or  hardcopy,   without   the  express   prior  written  permission  of
#  Sprite Entertainment, Inc.  This  copyright  notice  does not imply any
#  actual or intended publication.
#
# $Log: sprDefaultAttr.py,v $
# Revision 1.3  2012/05/10 00:29:50  tetsuya
# Added a line for the node which has no keyable attribute.
#
# Revision 1.2  2012/05/03 04:13:22  tetsuya
# Locked default attributes.
#
# Revision 1.1.1.1  2012/05/02 19:17:26  tetsuya
# Created by Pouya.
#
#
#  Using Scripts:
#

import maya.cmds as cmds
import pymel.core as pm

suffix = ''
prefix = 'default_'



def createDefaultAttributes(controllers):
	for controller in controllers:
		if nodeHasDefaultValue(controller):
			deleteDefaultAttributes(controller)
	for controller in controllers:
		#list attributes
		attributes = cmds.listAttr(controller,keyable=True,unlocked=True)
		if attributes != None:
			#create new default attributes
			for attr in attributes:
				#find attribute types
				attrType = cmds.ls((controller + '.' + attr),showType=True)
				#get attribute values
				if attrType[1] == 'enum':
					enumOptions = cmds.attributeQuery(attr, node=controller,listEnum=True)
					enumValue = cmds.getAttr(controller + '.' + attr)
					cmds.addAttr(controller,longName=(prefix + attr + suffix),attributeType='enum',enumName=enumOptions[0],keyable=False)
					cmds.setAttr((controller + '.' + prefix + attr + suffix),enumValue)
					cmds.setAttr((controller + '.' + prefix + attr + suffix),lock=True)
				else:
					value = cmds.getAttr(controller + '.' + attr)
					cmds.addAttr(controller, longName=(prefix + attr + suffix),attributeType=attrType[1], keyable=False)
					cmds.setAttr((controller + '.' + prefix + attr + suffix),value)
					cmds.setAttr((controller + '.' + prefix + attr + suffix),lock=True)
		else:
			cmds.warning(controller + ' doesn\'t have any children attribute, no default attribute for this controler')
						
		
		
		
		

    
    
def setAttributeValuesToDefault(controllers):
	for controller in controllers:
		channelBoxAttributes = checkIfDefaultAttrExistsInChannelBox(controller)
		for channelBoxAttribute in channelBoxAttributes:
		    if cmds.getAttr(controller + '.' + channelBoxAttribute,keyable=True)==False or cmds.getAttr(controller + '.' + channelBoxAttribute,lock=True) ==True:
		        cmds.warning(controller + '.' + channelBoxAttribute + ' is locked or not keyable.its LOCK or KEYABLE status has changes.')
		    else:
				if checkIfAttributeIsSameType(controller,channelBoxAttribute):
					#find default value
					defaultValue = cmds.getAttr(controller + '.' + prefix + channelBoxAttribute + suffix)
					cmds.setAttr((controller + '.' + channelBoxAttribute), defaultValue)
				else:
					cmds.warning(controller + '.' + channelBoxAttribute + ' is not same as it\'s default attribute, it\'s type has probebly changed at some point.plase double check.')



def nodeHasDefaultValue(controller):
	attributes = cmds.listAttr(controller)
	for attr in attributes:
		if cmds.attributeQuery((prefix + attr + suffix), node=controller,exists=True):
			return True
			break
			
def deleteDefaultAttributes(controller):
	attributes = cmds.listAttr(controller)
	for attr in attributes:
		if cmds.attributeQuery((prefix + attr + suffix), node=controller,exists=True):
			cmds.setAttr((controller + '.' + prefix + attr + suffix), lock=False)
			cmds.deleteAttr(controller, at=(prefix + attr + suffix))
			

		
		
def checkIfAttributeIsSameType(controller,attribute):
	#check default and current attribute types
	attributeType = cmds.ls((controller + '.' + attribute),showType=True)
	defaultAttributeType = cmds.ls((controller + '.' + prefix + attribute + suffix),showType=True)
	
	#if they are not the same return False and give warning
	if attributeType[1] == defaultAttributeType[1]: 
		return True	
    
def findDefaultAttributes(controller): 
	allAttributes = cmds.listAttr(controller)
	noSuffixPrefixDefaultAttrs = []
	for attr in allAttributes:
		if prefix != '' and attr.find(prefix) == 0:
			noPrefixDefaultAttr = attr.rpartition(prefix)[2]
			
			if suffix != '':
				noSuffixPrefixDefaultAttr = (noPrefixDefaultAttr.rpartition(suffix))[0]
				noSuffixPrefixDefaultAttrs.append(noSuffixPrefixDefaultAttr)
			else:
				noSuffixPrefixDefaultAttr = noPrefixDefaultAttr
				noSuffixPrefixDefaultAttrs.append(noSuffixPrefixDefaultAttr)
					
		elif suffix != '' and attr.find(suffix) == 0:
			noSuffixdefaultAttr = attr.rpartition(suffix)[0]
			
			if prefix != '':
				noSuffixPrefixDefaultAttr = (noSuffixdefaultAttr.rpartition(prefix))[2]
				noSuffixPrefixDefaultAttrs.append(noSuffixPrefixDefaultAttr)
			else:
				noSuffixPrefixDefaultAttr = noSuffixdefaultAttr
				noSuffixPrefixDefaultAttrs.append(noSuffixPrefixDefaultAttr)
			
	return noSuffixPrefixDefaultAttrs
	
def checkIfDefaultAttrExistsInChannelBox(controller):
	#list all the attributes
	attributes = cmds.listAttr(controller)
	#list default attributes
	defaultAttributes = findDefaultAttributes(controller)
	#lists attributes in channelBox
	channelBoxAttributes = []
	channelBoxAttributes.extend(cmds.listAttr(controller, keyable=True))
	if cmds.listAttr(controller, channelBox=True):
	    channelBoxAttributes.extend(cmds.listAttr(controller, channelBox=True))
	#check if each default attribute exists in channel box
	for defaultAttribute in defaultAttributes:
		if defaultAttribute not in  channelBoxAttributes:
			cmds.warning(controller + '.' + defaultAttribute + ' is hidden from channel box')
	return channelBoxAttributes
			
			
		
			
	
		
		
		
		
#to execute commands

#create default values
# selected = cmds.ls(selection=True)
# createDefaultAttributes(selected)

#set to default
#setAttributeValuesToDefault(selected)

#to delete default attributes
#selected = cmds.ls(selection=True)
#deleteDefaultAttributes(selected)
