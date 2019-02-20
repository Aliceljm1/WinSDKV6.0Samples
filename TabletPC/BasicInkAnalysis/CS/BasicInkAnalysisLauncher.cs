//-------------------------------------------------------------------------
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
//  Copyright (C) 2005 Microsoft Corporation
//  All rights reserved.
//
//  This source code is only intended as a supplement to the
//  Microsoft Tablet PC Platform SDK Reference and related electronic 
//  documentation provided with the Software Development Kit.
//  See these sources for more detailed information. 
//
//  File: BasicInkAnalysisSampleLauncher.cs
//  Simple Ink Analyzer Sample Application
//
//  This sample is based on the Ink Collection sample. An InkAnalyzer 
//  is created and asked to do layout analysis. In Automatic Layout 
//  Analysis mode (accessible through Mode->Automatic Layout Analysis 
//  menu item) the Ink Analyzer is invoked whenever the ink is changed. 
//  Otherwise, the Ink Analyzer can be invoked through File->Analyze 
//  menu item. In either case, the default recognizer is used, if 
//  available. The analyze method is called. The UI is then updated by 
//  drawing a bounding rectangle around each parsed unit. The type of
//  bounding box can be changed from horizontal to rotated by selecting
//  the Menu->RotatedBoundingBox option.  Besides using 
//  different colors, these rectangles are inflated by different amounts 
//  to ensure that none of the rectangles is obscured by others. 
//  The following table specifies the color and inflation for each units:
//
//      |-------------------|-----------|-----------|
//      | Unit              | Color     | Inflation |
//      |-------------------|-----------|-----------|
//      | Word              | Green     |     1     |
//      |-------------------|-----------|-----------|
//      | Line              | Magenta   |     3     |
//      |-------------------|-----------|-----------|
//      | Paragraph         | Blue      |     5     |
//      |-------------------|-----------|-----------|
//      | Writing Region    | Yellow    |     7     |
//      |-------------------|-----------|-----------|
//      | Drawing           | Red       |     1     |
//      |-------------------|-----------|-----------|
//
//  The application has the capability of erasing strokes. Two menu items
//  Mode->Ink and Mode->Erase are provided to switch between inking and
//  erasing modes. As the new ink is added or deleted to or from the Ink 
//  object, the ink analyzer is kept updated. 
//  The menu item for Mode->Automatic Layout Analysis is checked by default.
//  With this option checked, the InkCollector object's Stroke and StrokeDeleted 
//  event handlers call this method every time a stroke is made or deleted. With 
//  more than a few strokes present, the call to the InkAnalyzer object's Analyze 
//  method creates a noticeable delay. In practice, call the Analyze method only 
//  when you need the result or use the BackgroundAnalyze method.
//
//  The features used are: InkAnalyzer, InkOverlay, Erasing Ink, 
//      InkCollectorStrokeEventHandler, InkOverlayStrokesDeletingEventHandler, 
//      InkOverlayStrokesDeletedEventHandler and InkSpaceToPixel.
//
//--------------------------------------------------------------------------

using System;
using System.Windows.Forms;

namespace Microsoft.Samples.TabletPC.BasicInkAnalysis
{
    public class BasicInkAnalysisSampleLauncher
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.Run(new BasicInkAnalysisSampleForm());
        }
    }
}
