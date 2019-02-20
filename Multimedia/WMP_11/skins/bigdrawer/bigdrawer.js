//    Copyright (c) Microsoft Corporation.  All rights reserved.

var g_isOpen = false;

function moveDrawer() {
	if (g_isOpen) {
		drawer.moveTo(80,0,500);
		drawerButton.upToolTip = "Show Video or Change Visualization";
		g_isOpen = false;
                hiddenvideo.visible = "false";
	} else {
		drawer.moveTo(0,0,500);
		drawerButton.upToolTip = "Close Drawer";
		g_isOpen = true;
                hiddenvideo.visible = "true";
	}
}

