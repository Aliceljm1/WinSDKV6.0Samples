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
//  File: BasicInkAnalysisSample.cs
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
//      | Bullet            | Orange    |     1     |
//      |-------------------|-----------|-----------|

//
//  The application has the capability of erasing strokes. Two menu items
//  Mode->Ink and Mode->Erase are provided to switch between inking and
//  erasing modes. As the new ink is added or deleted to or from the Ink 
//  object, the ink analyzer is kept updated. 
//  The menu item for Mode->Automatic Layout Analysis is checked by default.
//  With this option checked, the InkOverlay object's Stroke and StrokeDeleting 
//  event handlers call this method every time a stroke is made or deleted. With 
//  more than a few strokes present, the call to the InkAnalyzer object's Analyze 
//  method creates a noticeable delay. In practice, call the Analyze method only 
//  when you need the result or use the BackgroundAnalyze method.
//
//  The features used are: InkAnalyzer, InkOverlay, Erasing Ink, 
//      InkCollectorStrokeEventHandler, InkOverlayStrokesDeletingEventHandler, 
//      and InkSpaceToPixel.
//
//--------------------------------------------------------------------------

using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

//   To use the Ink and Ink Analyzer functionality, you need to add the following 
//   references to dll's into your solution: IACore.dll, IALoader.dll, 
//   Microsoft.Ink.Analysis.dll, and Microsoft.Ink.dll  
using Microsoft.Ink;

namespace Microsoft.Samples.TabletPC.BasicInkAnalysis
{
    public class BasicInkAnalysisSampleForm : Form
    {

        #region Declarations
        // Declare the Ink Overlay object
        private InkOverlay myInkOverlay;

        // Declare the InkAnalyzer object
        private InkAnalyzer myInkAnalyzer;

        // Declare arrays of rectangles for the Horizontal Bounding Boxes 
        // for words, drawings, lines, paragraphs and writing regions
        Rectangle[] myWordHorizontalBBoxes;
        Rectangle[] myDrawingHorizontalBBoxes;
        Rectangle[] myBulletHorizontalBBoxes;
        Rectangle[] myLineHorizontalBBoxes;
        Rectangle[] myParagraphHorizontalBBoxes;
        Rectangle[] myWritingRegionHorizontalBoundingBoxes;

        // Declare an ArrayList of Rotated Bounding Boxes for 
        // words, drawings, lines, paragraphs and writing regions
        ArrayList myWordRotatedBoundingBoxes;
        ArrayList myDrawingRotatedBoundingBoxes;
        ArrayList myBulletRotatedBoundingBoxes;
        ArrayList myLineRotatedBoundingBoxes;
        ArrayList myParagraphRotatedBoundingBoxes;
        ArrayList myWritingRegionRotatedBoundingBoxes;

        // Declare the pen we will use to draw the bounding boxes
        Pen penBox;
        #endregion

        #region Standard Template Code
        // Menu and menu items to control the program state
        private System.Windows.Forms.MainMenu mainMenu;
        private System.Windows.Forms.MenuItem miFile;
        private System.Windows.Forms.MenuItem miAnalyze;
        private System.Windows.Forms.MenuItem miExit;
        private System.Windows.Forms.MenuItem miMode;
        private System.Windows.Forms.MenuItem miInk;
        private System.Windows.Forms.MenuItem miErase;
        private System.Windows.Forms.MenuItem miAutomaticLayoutAnalysis;
        private System.Windows.Forms.MenuItem miSeparate;
        private MenuItem miSeparate2;
        private MenuItem miRotatedBoundingBoxes;

        // Status bar to display the meaning of each color
        private System.Windows.Forms.StatusBar statusBar;
        private System.Windows.Forms.StatusBarPanel statusBarPanelDummy;
        private System.Windows.Forms.StatusBarPanel statusBarPanelDrawing;
        private System.Windows.Forms.StatusBarPanel statusBarPanelBullet;
        private System.Windows.Forms.StatusBarPanel statusBarPanelParagraph;
        private System.Windows.Forms.StatusBarPanel statusBarPanelWord;
        private System.Windows.Forms.StatusBarPanel statusBarPanelWritingRegion;
        private System.Windows.Forms.StatusBarPanel statusBarPanelLine;

        // Panel to attach the InkOverlay
        private System.Windows.Forms.Panel drawArea;

        // Labeled text box to display analysis results
        private Label analysisResultLabel;
        private TextBox analysisResult;
        private Panel analysisResultPanel;

        // Standard storage for the components contained in this frame
        private System.ComponentModel.IContainer components;

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.mainMenu = new System.Windows.Forms.MainMenu();
            this.miFile = new System.Windows.Forms.MenuItem();
            this.miAnalyze = new System.Windows.Forms.MenuItem();
            this.miExit = new System.Windows.Forms.MenuItem();
            this.miMode = new System.Windows.Forms.MenuItem();
            this.miInk = new System.Windows.Forms.MenuItem();
            this.miErase = new System.Windows.Forms.MenuItem();
            this.miSeparate = new System.Windows.Forms.MenuItem();
            this.miAutomaticLayoutAnalysis = new System.Windows.Forms.MenuItem();
            this.miSeparate2 = new System.Windows.Forms.MenuItem();
            this.miRotatedBoundingBoxes = new System.Windows.Forms.MenuItem();
            this.drawArea = new System.Windows.Forms.Panel();
            this.statusBar = new System.Windows.Forms.StatusBar();
            this.statusBarPanelDummy = new System.Windows.Forms.StatusBarPanel();
            this.statusBarPanelWord = new System.Windows.Forms.StatusBarPanel();
            this.statusBarPanelLine = new System.Windows.Forms.StatusBarPanel();
            this.statusBarPanelParagraph = new System.Windows.Forms.StatusBarPanel();
            this.statusBarPanelWritingRegion = new System.Windows.Forms.StatusBarPanel();
            this.statusBarPanelDrawing = new System.Windows.Forms.StatusBarPanel();
            this.statusBarPanelBullet = new System.Windows.Forms.StatusBarPanel();
            this.analysisResult = new System.Windows.Forms.TextBox();
            this.analysisResultLabel = new System.Windows.Forms.Label();
            this.analysisResultPanel = new System.Windows.Forms.Panel();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelDummy)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelWord)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelLine)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelParagraph)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelWritingRegion)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelDrawing)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelDrawing)).BeginInit();
            this.analysisResultPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainMenu
            // 
            this.mainMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
                                                                                     this.miFile,
                                                                                     this.miMode});
            // 
            // miFile
            // 
            this.miFile.Index = 0;
            this.miFile.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
                                                                                   this.miAnalyze,
                                                                                   this.miExit});
            this.miFile.Text = "&File";
            // 
            // miAnalyze
            // 
            this.miAnalyze.Index = 0;
            this.miAnalyze.Text = "&Analyze";
            this.miAnalyze.Click += new System.EventHandler(this.miAnalyze_Click);
            // 
            // miExit
            // 
            this.miExit.Index = 1;
            this.miExit.Text = "E&xit";
            this.miExit.Click += new System.EventHandler(this.miExit_Click);
            // 
            // miMode
            // 
            this.miMode.Index = 1;
            this.miMode.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
                                                                                   this.miInk,
                                                                                   this.miErase,
                                                                                   this.miSeparate,
                                                                                   this.miAutomaticLayoutAnalysis,
                                                                                   this.miSeparate2,
                                                                                   this.miRotatedBoundingBoxes});
            this.miMode.Text = "&Mode";
            // 
            // miInk
            // 
            this.miInk.Index = 0;
            this.miInk.Text = "&Ink";
            this.miInk.Click += new System.EventHandler(this.miInk_Click);
            // 
            // miErase
            // 
            this.miErase.Index = 1;
            this.miErase.Text = "&Erase";
            this.miErase.Click += new System.EventHandler(this.miErase_Click);
            // 
            // miSeparate
            // 
            this.miSeparate.Index = 2;
            this.miSeparate.Text = "-";
            // 
            // miAutomaticLayoutAnalysis
            // 
            this.miAutomaticLayoutAnalysis.Checked = true;
            this.miAutomaticLayoutAnalysis.Index = 3;
            this.miAutomaticLayoutAnalysis.Text = "&Automatic Layout Analysis";
            this.miAutomaticLayoutAnalysis.Click += new System.EventHandler(this.miAutomaticLayoutAnalysis_Click);
            // 
            // miSeparate2
            // 
            this.miSeparate2.Index = 4;
            this.miSeparate2.Text = "-";
            // 
            // miRotatedBoundingBoxes
            // 
            this.miRotatedBoundingBoxes.Index = 5;
            this.miRotatedBoundingBoxes.Text = "&Rotated Bounding Boxes";
            this.miRotatedBoundingBoxes.Click += new System.EventHandler(this.miRotatedBoundingBox_Click);
            // 
            // drawArea
            // 
            this.drawArea.BackColor = System.Drawing.SystemColors.Window;
            this.drawArea.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.drawArea.Dock = System.Windows.Forms.DockStyle.Fill;
            this.drawArea.Location = new System.Drawing.Point(0, 0);
            this.drawArea.Name = "drawArea";
            this.drawArea.Size = new System.Drawing.Size(672, 441);
            this.drawArea.TabIndex = 2;
            this.drawArea.Paint += new System.Windows.Forms.PaintEventHandler(this.DrawArea_Paint);
            // 
            // statusBar
            // 
            this.statusBar.Location = new System.Drawing.Point(0, 420);
            this.statusBar.Name = "statusBar";
            this.statusBar.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
                                                                                         this.statusBarPanelDummy,
                                                                                         this.statusBarPanelWord,
                                                                                         this.statusBarPanelLine,
                                                                                         this.statusBarPanelParagraph,
                                                                                         this.statusBarPanelWritingRegion,
                                                                                         this.statusBarPanelDrawing,
                                                                                         this.statusBarPanelBullet} );
            this.statusBar.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.statusBar.ShowPanels = true;
            this.statusBar.Size = new System.Drawing.Size(672, 21);
            this.statusBar.TabIndex = 3;
            // 
            // statusBarPanelDummy
            // 
            this.statusBarPanelDummy.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Spring;
            this.statusBarPanelDummy.BorderStyle = System.Windows.Forms.StatusBarPanelBorderStyle.None;
            this.statusBarPanelDummy.Width = 184;
            // 
            // statusBarPanelWord
            // 
            this.statusBarPanelWord.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents;
            this.statusBarPanelWord.Text = "Green: Word";
            this.statusBarPanelWord.Width = 79;
            // 
            // statusBarPanelLine
            // 
            this.statusBarPanelLine.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents;
            this.statusBarPanelLine.Text = "Magenta: Line";
            this.statusBarPanelLine.Width = 86;
            // 
            // statusBarPanelParagraph
            // 
            this.statusBarPanelParagraph.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents;
            this.statusBarPanelParagraph.Text = "Blue: Paragraph";
            this.statusBarPanelParagraph.Width = 96;
            // 
            // statusBarPanelWritingRegion
            // 
            this.statusBarPanelWritingRegion.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents;
            this.statusBarPanelWritingRegion.Text = "Yellow: Writing Region";
            this.statusBarPanelWritingRegion.Width = 128;
            // 
            // statusBarPanelDrawing
            // 
            this.statusBarPanelDrawing.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents;
            this.statusBarPanelDrawing.Text = "Red: Drawing";
            this.statusBarPanelDrawing.Width = 83;

            // 
            // statusBarPanelBullet
            // 
            this.statusBarPanelBullet.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents;
            this.statusBarPanelBullet.Text = "Orange: Bullet";
            this.statusBarPanelBullet.Width = 83;

            // 
            // analysisResult
            // 
            this.analysisResult.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
                | System.Windows.Forms.AnchorStyles.Left) 
                | System.Windows.Forms.AnchorStyles.Right)));
            this.analysisResult.BackColor = System.Drawing.Color.White;
            this.analysisResult.Location = new System.Drawing.Point(117, 12);
            this.analysisResult.Multiline = true;
            this.analysisResult.Name = "analysisResult";
            this.analysisResult.ReadOnly = true;
            this.analysisResult.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.analysisResult.Size = new System.Drawing.Size(542, 74);
            this.analysisResult.TabIndex = 4;
            this.analysisResult.Text = "";
            this.analysisResult.WordWrap = false;
            // 
            // analysisResultLabel
            // 
            this.analysisResultLabel.AutoSize = true;
            this.analysisResultLabel.Location = new System.Drawing.Point(13, 36);
            this.analysisResultLabel.Name = "analysisResultLabel";
            this.analysisResultLabel.Size = new System.Drawing.Size(85, 16);
            this.analysisResultLabel.TabIndex = 5;
            this.analysisResultLabel.Text = "Analysis Result:";
            // 
            // analysisResultPanel
            // 
            this.analysisResultPanel.Controls.Add(this.analysisResult);
            this.analysisResultPanel.Controls.Add(this.analysisResultLabel);
            this.analysisResultPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.analysisResultPanel.Location = new System.Drawing.Point(0, 322);
            this.analysisResultPanel.Name = "analysisResultPanel";
            this.analysisResultPanel.Size = new System.Drawing.Size(672, 98);
            this.analysisResultPanel.TabIndex = 4;
            // 
            // BasicInkAnalysisSampleForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(672, 441);
            this.Controls.Add(this.analysisResultPanel);
            this.Controls.Add(this.statusBar);
            this.Controls.Add(this.drawArea);
            this.Menu = this.mainMenu;
            this.Name = "BasicInkAnalysisSampleForm";
            this.Text = "Basic Ink Analysis";
            this.Load += new System.EventHandler(this.InkAnalyzerForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelDummy)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelWord)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelLine)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelParagraph)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelWritingRegion)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelDrawing)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanelBullet)).EndInit();
            this.analysisResultPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }
        #endregion

        #region Form Load Code
        /// <summary>
        /// Event Handler from Form Load Event
        /// Setup the ink overlay for collection
        /// </summary>
        /// <param name="sender">The control that raised the event.</param>
        /// <param name="e">The event arguments.</param>
        private void InkAnalyzerForm_Load(object sender, System.EventArgs e)
        {
            // Create the ink overlay and associate it with the form
            myInkOverlay = new Microsoft.Ink.InkOverlay(drawArea.Handle);

            // Hook event handler for the Stroke event to myInkOverlay_Stroke.
            // This is necessary since the application needs to pass the strokes 
            // to the InkAnalyzer.
            myInkOverlay.Stroke += new InkCollectorStrokeEventHandler(myInkOverlay_Stroke);

            // Hook the event handler for StrokeDeleting event to myInkOverlay_StrokeDeleting.
            // This is necessary as the application needs to remove the strokes from 
            // the InkAnalyzer object as well.
            myInkOverlay.StrokesDeleting += new InkOverlayStrokesDeletingEventHandler(myInkOverlay_StrokeDeleting);

            // Create a instance of the InkAnalyzer object and 
            // associate an ink object to use. 
            myInkAnalyzer = new InkAnalyzer(myInkOverlay.Ink, this);
            myInkAnalyzer.SynchronizingObject = this;

            // Hook the event handler for ResultsUpdatedEvent event to myInkAnalyzer_Results.
            // This is necessary to update the way bounding boxes are drawn and the 
            // text that appears in the analysisResults text box.
            myInkAnalyzer.ResultsUpdated += new ResultsUpdatedEventHandler ( myInkAnalyzer_Results );

            // Enable ink collection
            myInkOverlay.Enabled = true;

            // Set check for ink menu item
            miInk.Checked = true;

            // Construct the pen that will be used for drawing.  Pen color is not important,
            // as it will be reset before it is used, but is a required parameter for the 
            // constructor.
            penBox = new Pen(Color.Black, 2.0f);
        }
        #endregion

        #region Handle Stroke Events
        /// <summary>
        /// Event Handler from Ink Overlay's Stroke event
        /// This event is fired when a new stroke is drawn. 
        /// In this case, it is necessary to update the ink analyzer's 
        /// strokes collection. The event is fired even when the eraser stroke is created.
        /// The event handler must filter out the eraser strokes.
        /// </summary>
        /// <param name="sender">The control that raised the event.</param>
        /// <param name="e">The event arguments.</param>
        private void myInkOverlay_Stroke(object sender, InkCollectorStrokeEventArgs e)
        {
            // Filter out the eraser stroke.
            if (InkOverlayEditingMode.Ink == myInkOverlay.EditingMode)
            {

                // Add the new stroke to the InkAnalyzer's stroke collection
                myInkAnalyzer.AddStroke(e.Stroke);

                if (miAutomaticLayoutAnalysis.Checked)
                {
                    // Invoke an analysis operation on the background thread
                    myInkAnalyzer.BackgroundAnalyze();
                }
            }
        }

        /// <summary>
        /// Event Handler for Ink Overlay's StrokeDeleting event. 
        /// This event is fired when a set of stroke is about to be deleted.
        /// The stroke should also be removed from the ink analyzer's
        /// stroke collection as well.  It is neccessary to remove the 
        /// stroke based on the StrokeDeleting Event instead of the StrokeDeleted
        /// event because by the time a StrokeDeleted event is fired, the stroke 
        /// has already been removed from the Ink object associated with
        /// our analyzer.
        /// </summary>
        /// <param name="sender">The control that raised the event</param>
        /// <param name="e">The event arguments</param>
        void myInkOverlay_StrokeDeleting(object sender, InkOverlayStrokesDeletingEventArgs e)
        {
            // Remove the strokes to be deleted from the InkAnalyzer's stroke collection
            myInkAnalyzer.RemoveStrokes(e.StrokesToDelete);

            // If automatic layout analysis is turned on, call AnalyzeInk
            if ( miAutomaticLayoutAnalysis.Checked )
            {
                // Invoke an analysis operation on the background thread
                myInkAnalyzer.BackgroundAnalyze ( );
            }
        }

        #endregion

        #region Handle Analysis Results
        /// <summary>
        /// This method is called when a background analyze operation 
        /// completes.  It uses the new results to update the bounding 
        /// boxes and the analysisResults text box.
        /// </summary>
        /// <param name="sender">The control that raised the event</param>
        /// <param name="e">The event argument</param>
        void myInkAnalyzer_Results ( object sender, ResultsUpdatedEventArgs e )
        {
            // Update the UI elements
            UpdateUI();
        }

        /// <summary>
        /// This method updates the UI elements in the form that change
        /// based on analysis results.  Horizontal and Rotated Bounding Boxes
        /// will be re-calculated, and the analysisResults text field will be
        /// reset to result
        /// </summary>
        /// <param name="result">The new text that will be displayed in 
        /// the analysisResults text field</param>
        private void UpdateUI()
        {
            // get the horizontal bounding boxes
            if (miRotatedBoundingBoxes.Checked)
            {
                CalculateRotatedBoundingBoxes();
            }

            // else calculate the rotated bounding boxes
            else
            {
                CalculateHorizontalBoundingBoxes();

            }
            // Update the drawable area to reflect these changes
            drawArea.Refresh();

            // Fetch the string that represents the recognized text in
            // the drawing area
            String result = myInkAnalyzer.GetRecognizedString();

            // Update the text in the analysis results text field
            if (result == null || result.Length == 0)
            {
                this.analysisResult.Text = "No Result Available";
            }
            else this.analysisResult.Text = result;

        }
        #endregion

        #region Calculate Bounding boxes
        /// <summary>
        /// This method calculates the values of all the nodes needed for the horizontal bounding boxes
        /// </summary>
        private void CalculateHorizontalBoundingBoxes()
        {

            // Call helper function to get the horizontal bounding boxes for Words
            // Rectangles are inflated by 1 pixel in all direction to 
            // avoid overlapping with stroke
            myWordHorizontalBBoxes = GetHorizontalBBoxes(ContextNodeType.InkWord, 1);

            // Call helper function to get the horizontal bounding boxes for Lines
            // Rectangles are inflated by 3 pixels in all directions
            myLineHorizontalBBoxes = GetHorizontalBBoxes(ContextNodeType.Line, 3);

            // Call helper function to get the horizontal bounding boxes for Paragraphs
            // Rectangles are inflated by 5 pixels in all directions
            myParagraphHorizontalBBoxes = GetHorizontalBBoxes(ContextNodeType.Paragraph, 5);

            // Call helper function to get the horizontal bounding boxes for Writing Regions
            // Rectangles are inflated by 7 pixels in all directions
            myWritingRegionHorizontalBoundingBoxes =
                                        GetHorizontalBBoxes(ContextNodeType.WritingRegion, 7);

            // Call helper function to get the bounding boxes for Drawings
            // The rectangles are inflated by 1 pixel in all directions
            myDrawingHorizontalBBoxes = GetHorizontalBBoxes(ContextNodeType.InkDrawing, 1);

            // Call helper function to get the bounding boxes for Bullets
            // The rectangles are inflated by 1 pixel in all directions
            myBulletHorizontalBBoxes = GetHorizontalBBoxes ( ContextNodeType.InkBullet, 1 );
        }

        /// <summary>
        /// This method calculates the values of all the nodes needed for the rotated bounding boxes
        /// </summary>
        private void CalculateRotatedBoundingBoxes()
        {
            // Call helper function to get the rotated bounding boxes for Words
            // Rectangles are inflated by 1 pixel in all direction to 
            // avoid overlapping with stroke
            myWordRotatedBoundingBoxes = GetRotatedBBoxes(ContextNodeType.InkWord, 1);

            // Call helper function to get the rotated bounding boxes for Lines
            // Rectangles are inflated by 3 pixels in all directions
            myLineRotatedBoundingBoxes = GetRotatedBBoxes(ContextNodeType.Line, 3);

            // Call helper function to get the rotated bounding boxes for Paragraphs
            // Rectangles are inflated by 5 pixels in all directions
            myParagraphRotatedBoundingBoxes = GetRotatedBBoxes(ContextNodeType.Paragraph, 5);

            // Call helper function to get the rotated bounding boxes for Writing Regions
            // Rectangles are inflated by 7 pixels in all directions
            myWritingRegionRotatedBoundingBoxes = GetRotatedBBoxes(ContextNodeType.WritingRegion, 7);

            // Call helper function to get the bounding boxes for Drawings
            // The rectangles are inflated by 1 pixel in all directions
            myDrawingRotatedBoundingBoxes = GetRotatedBBoxes(ContextNodeType.InkDrawing, 1);

            // Call helper function to get the bounding boxes for Bullets
            // The rectangles are inflated by 1 pixel in all directions
            myBulletRotatedBoundingBoxes = GetRotatedBBoxes ( ContextNodeType.InkBullet, 1 );
        }

        #region Get Horizontal Rectangles to Draw from Analyzer
        /// <summary>
        /// Helper function to obtain array of rectangles from the 
        /// division result of the division type of interest. Each rectangle
        /// is inflated by the amount specified in the third parameter. This
        /// is done to ensure the visibility of all rectangles.
        /// </summary>
        /// <param name="nodeType">ContextNode type</param>
        /// <param name="inflate">Number of Pixels by which the rectangles are inflated</param>
        /// <returns> Array of rectangles containing bounding boxes of 
        /// division type specified by divType. The rectangles are in pixel unit.</returns>
        private Rectangle[] GetHorizontalBBoxes(Guid nodeType, int inflate)
        {
            // Declare the array of rectangles to hold the result
            Rectangle[] analysisRects;

            // Get the division units from the division result of division type
            ContextNodeCollection nodes = myInkAnalyzer.FindNodesOfType(nodeType);

            // If there is at least one unit, we construct the rectangles
            if ((null != nodes) && (0 < nodes.Count))
            {
                // We need to convert rectangles from ink units to
                // pixel units. For that, we need Graphics object
                // to pass to InkRenderer.InkSpaceToPixel method
                using (Graphics g = drawArea.CreateGraphics())
                {
                    // Construct the rectangles
                    analysisRects = new Rectangle[nodes.Count];

                    // InkRenderer.InkSpaceToPixel takes Point as parameter. 
                    // Create two Point objects to point to (Top, Left) and
                    // (Width, Height) properties of ractangle. (Width, Height)
                    // is used instead of (Right, Bottom) because (Right, Bottom)
                    // are read-only properties on Rectangle
                    Point ptLocation = new Point();
                    Point ptSize = new Point();

                    // Index into the bounding boxes
                    int i = 0;

                    // Iterate through the collection of division units to obtain the bounding boxes
                    foreach (ContextNode node in nodes)
                    {
                        // Get the bounding box of the strokes of the division unit
                        analysisRects[i] = node.Location.GetBounds();

                        // The bounding box is in ink space unit. Convert them into pixel unit. 
                        ptLocation = analysisRects[i].Location;
                        ptSize.X = analysisRects[i].Width;
                        ptSize.Y = analysisRects[i].Height;

                        // Convert the Location from Ink Space to Pixel Space
                        myInkOverlay.Renderer.InkSpaceToPixel(g, ref ptLocation);

                        // Convert the Size from Ink Space to Pixel Space
                        myInkOverlay.Renderer.InkSpaceToPixel(g, ref ptSize);

                        // Assign the result back to the corresponding properties
                        analysisRects[i].Location = ptLocation;
                        analysisRects[i].Width = ptSize.X;
                        analysisRects[i].Height = ptSize.Y;

                        // Inflate the rectangle by inflate pixels in both directions
                        analysisRects[i].Inflate(inflate, inflate);

                        // Increment the index
                        ++i;
                    }

                } // Relinquish the Graphics object
            }
            else
            {
                // Otherwise we return null
                analysisRects = null;
            }

            // Return the Rectangle[] object
            return analysisRects;
        }
        #endregion

        #region Get Rotated Points[] to Draw from Analyzer

        /// <summary>
        /// Gets the rotated bounding boxes for the specified node type and inflates the 
        /// value by the specified amount
        /// </summary>
        /// <param name="nodeType">Sepcifies the node type to retrieve</param>
        /// <param name="inflate"></param>
        /// <returns></returns>
        private System.Collections.ArrayList GetRotatedBBoxes(Guid nodeType, int inflate)
        {
            //Find the correct collection of results nodes.
            ContextNodeCollection Nodes = myInkAnalyzer.FindNodesOfType(nodeType);

            // Declare the array list to hold the results; 
            // This array represents the four points of a rectangle, with the first point
            // copied again to complete the cycle of points.

            ArrayList polygonPoints = new ArrayList(Nodes.Count);

            // Cycle through each results node, get and convert the
            // rotated bounding box points
            foreach (ContextNode node in Nodes)
            {
                //Declare the point array
                Point[] rotatedBoundingBox = null;

                //Switch on the type of ContextNode to cast into the
                //appropriate type.  This is required to access the 
                //type specific property "RotatedBoundingBox" which
                //is not found on all ContextNode types.
                if (nodeType == ContextNodeType.InkWord)
                {
                    rotatedBoundingBox = ((InkWordNode)node).GetRotatedBoundingBox();
                }
                else if (nodeType == ContextNodeType.Line)
                {
                    rotatedBoundingBox = ((LineNode)node).GetRotatedBoundingBox();
                }
                else if (nodeType == ContextNodeType.Paragraph)
                {
                    rotatedBoundingBox = ((ParagraphNode)node).GetRotatedBoundingBox();
                }
                else if (nodeType == ContextNodeType.WritingRegion ||
                         nodeType == ContextNodeType.InkDrawing ||
                         nodeType == ContextNodeType.InkBullet )
                {

                    // Rotated Bounding Boxes are not a supported option for 
                    // Writing Regions or Drawings.  We return the axis aligned 
                    // bounding box instead
                    Rectangle rect = node.Location.GetBounds();

                    // We need to create a looped list of 4 points to be consistent
                    // with the way InkAnalysis represents rotated bounding boxes.  
                    rotatedBoundingBox = new Point[4];

                    rotatedBoundingBox[0] = new Point(rect.X, rect.Y);
                    rotatedBoundingBox[1] = new Point(rect.Right, rect.Y);
                    rotatedBoundingBox[2] = new Point(rect.Right, rect.Bottom);
                    rotatedBoundingBox[3] = new Point(rect.X, rect.Bottom);

                }


                if (null != rotatedBoundingBox)
                {
                    // We need to convert rectangles from ink units to
                    // pixel units. For that, we need Graphics object
                    // to pass to InkRenderer.InkSpaceToPixel method
                    using (Graphics g = drawArea.CreateGraphics())
                    {
                        // convert each of the points from ink space to pixel space
                        for (int i = 0; i < rotatedBoundingBox.Length; i++)
                        {
                            myInkOverlay.Renderer.InkSpaceToPixel(g, ref rotatedBoundingBox[i]);
                        }

                        //inflate the points by calling helper method
                        InflateHelperMethod(ref rotatedBoundingBox, inflate);

                        // increment the node portion of the polygonPoints array
                        polygonPoints.Add(rotatedBoundingBox);
                    }
                }
            }
            //Return the results
            return polygonPoints;
        }
        #endregion

        #region Inflation Helper
        /// <summary>
        /// Inflation helper needed to inflate rotated bounding boxes which are comprised
        /// of four points.  
        /// </summary>
        /// <param name="points">Point array of size 4 </param>
        /// <param name="inflateValue">inflation value point array will be increase by</param>
        private static void InflateHelperMethod(ref Point[] points, int inflateValue)
        {

            // Inflate the first and third points' X coordinate by the inflation value
            
            if (points[0].X < points[2].X)
            {
                points[0].X -= inflateValue;

                points[2].X += inflateValue;

            }
            else
            {
                points[0].X += inflateValue;

                points[2].X -= inflateValue;
            }

            //Inflate the first and third points' Y coordinate by the inflation value

            if (points[0].Y < points[2].Y)
            {
                points[0].Y -= inflateValue;


                points[2].Y += inflateValue;
            }
            else
            {
                points[0].Y += inflateValue;


                points[2].Y -= inflateValue;
            }

            //Inflate the second and fourth points' X coordinate by the inflation value
            if (points[1].X > points[3].X)
            {
                points[1].X += inflateValue;

                points[3].X -= inflateValue;
            }
            else
            {
                points[1].X -= inflateValue;

                points[3].X += inflateValue;
            }

            //Inflate the second and fourth points' Y coordinate by the inflation value
            if (points[1].Y < points[3].Y)
            {
                points[1].Y -= inflateValue;

                points[3].Y += inflateValue;
            }
            else
            {
                points[1].Y += inflateValue;

                points[3].Y -= inflateValue;
            }

        }
        #endregion

        #endregion

        #region Paint
        /// <summary>
        /// Paint method gets called everytime when the window is refreshed.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DrawArea_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {

            #region Draw Horizontal Bounding Boxes
            //Draw Horizontal Bounding Boxes if menu item 
            //"Rotated Bounding Boxes" is unchecked
            if (!miRotatedBoundingBoxes.Checked)
            {

                // First, draw the bounding boxes for Writing Regions
                if (null != myWritingRegionHorizontalBoundingBoxes)
                {
                    // Change the pen color to Yellow 
                    penBox.Color = Color.Yellow;

                    // Draw bounding boxes for Writing Regions
                    e.Graphics.DrawRectangles(penBox, myWritingRegionHorizontalBoundingBoxes);
                }

                // Second, draw the bounding boxes for Paragraphs
                if (null != myParagraphHorizontalBBoxes)
                {
                    // Change the pen color to Blue
                    penBox.Color = Color.Blue;
                    // Draw bounding boxes for Paragraphs
                    e.Graphics.DrawRectangles(penBox, myParagraphHorizontalBBoxes);
                }

                // Next, draw the bounding boxes for Lines
                if (null != myLineHorizontalBBoxes)
                {
                    // Change the pen color to Magenta
                    penBox.Color = Color.Magenta;
                    // Draw the bounding boxes for Lines
                    e.Graphics.DrawRectangles(penBox, myLineHorizontalBBoxes);
                }

                // Then, draw the bounding boxes for Words
                if (null != myWordHorizontalBBoxes)
                {
                    // Change the pen color to Green
                    penBox.Color = Color.Green;
                    // Draw bounding boxes for Words
                    e.Graphics.DrawRectangles(penBox, myWordHorizontalBBoxes);
                }

                // Then, draw the boxes for Drawings
                if (null != myDrawingHorizontalBBoxes)
                {
                    // Change the pen color to Red
                    penBox.Color = Color.Red;
                    // Draw bounding boxes for Drawings
                    e.Graphics.DrawRectangles(penBox, myDrawingHorizontalBBoxes);
                }

                // Finally, draw the boxes for Bullets
                if ( null != myBulletHorizontalBBoxes )
                {
                    // Change the pen color to Orange
                    penBox.Color = Color.Orange;
                    // Draw bounding boxes for Drawings
                    e.Graphics.DrawRectangles ( penBox, myBulletHorizontalBBoxes );
                }

            }

            #endregion

            #region Draw Rotated Bounding Boxes
            //Draw Rotated bounding boxes as "Rotated Bounding Boxes" is
            //checked in menu
            else
            {
                // First, draw the bounding boxes for Writing Regions
                if (null != myWritingRegionRotatedBoundingBoxes)
                {
                    // Color is Yellow pen
                    penBox.Color = Color.Yellow;

                    // Since there is no "DrawPolygons" method, loop through
                    // and draw each Writing Region node's rotated bounding box
                    foreach (Point[] pt in myWritingRegionRotatedBoundingBoxes)
                    {
                        // Draw bounding boxes for Writing Regions
                        e.Graphics.DrawPolygon(penBox, pt);
                    }
                }
                // Second, draw the bounding boxes for Paragraphs
                if (null != myParagraphRotatedBoundingBoxes)
                {
                    // Change the pen color to Blue
                    penBox.Color = Color.Blue;

                    // Since there is no "DrawPolygons" method, loop through
                    // and draw each Paragraph node's rotated bounding box
                    foreach (Point[] pt in myParagraphRotatedBoundingBoxes)
                    {
                        // Draw bounding boxes for Paragraphs
                        e.Graphics.DrawPolygon(penBox, pt);
                    }
                }

                // Next, draw the bounding boxes for Lines
                if (null != myLineRotatedBoundingBoxes)
                {
                    // Change the pen color to Magenta
                    penBox.Color = Color.Magenta;

                    // Since there is no "DrawPolygons" method, loop through
                    // and draw each Line node's rotated bounding box
                    foreach (Point[] pt in myLineRotatedBoundingBoxes)
                    {
                        // Draw bounding boxes for Line
                        e.Graphics.DrawPolygon(penBox, pt);
                    }
                }

                // Then, draw the bounding boxes for Words
                if (null != myWordRotatedBoundingBoxes)
                {
                    // Change the pen color to Green
                    penBox.Color = Color.Green;

                    // Since there is no "DrawPolygons" method, loop through
                    // and draw each Word node's rotated bounding box
                    foreach (Point[] pt in myWordRotatedBoundingBoxes)
                    {
                        // Draw bounding boxes for Word
                        e.Graphics.DrawPolygon(penBox, pt);
                    }

                }

                // Then, draw the boxes for Drawings
                if (null != myDrawingRotatedBoundingBoxes)
                {
                    // Change the pen color to Red
                    penBox.Color = Color.Red;
                    // Since there is no "DrawPolygons" method, loop through
                    // and draw each Drawing node's rotated bounding box
                    foreach (Point[] pt in myDrawingRotatedBoundingBoxes)
                    {
                        // Draw bounding boxes for Drawing
                        e.Graphics.DrawPolygon(penBox, pt);
                    }
                }

                // Finally, draw the boxes for Bullets
                if ( null != myDrawingRotatedBoundingBoxes )
                {
                    // Change the pen color to Orange
                    penBox.Color = Color.Orange;
                    // Since there is no "DrawPolygons" method, loop through
                    // and draw each Bullet node's rotated bounding box
                    foreach ( Point [ ] pt in myBulletRotatedBoundingBoxes )
                    {
                        // Draw bounding boxes for Bullet
                        e.Graphics.DrawPolygon ( penBox, pt );
                    }
                }

            }
            #endregion

        }
        #endregion

        #region Other Form Handlers
        /// <summary>
        /// Event handler for File->Analyze menu item
        /// </summary>
        /// <param name="sender">The control that raised the event.</param>
        /// <param name="e">The event arguments.</param>
        private void miAnalyze_Click(object sender, System.EventArgs e)
        {
            // Call AnalyzeInk to perform the analysis on this thread
            AnalysisStatus status = myInkAnalyzer.Analyze();

            // If the analysis was successful
            if (status.Successful)
            {
                // Update the UI elements to reflect the new results.
                UpdateUI();
            }


        }

        /// <summary>
        /// Event handler for File->Exit menu item
        /// </summary>
        /// <param name="sender">The control that raised the event.</param>
        /// <param name="e">The event arguments.</param>
        private void miExit_Click(object sender, System.EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// Event handler for Mode->Ink menu item
        /// </summary>
        /// <param name="sender">The control that raised the event.</param>
        /// <param name="e">The event arguments.</param>
        private void miInk_Click(object sender, System.EventArgs e)
        {
            // Turn on the inking mode
            myInkOverlay.EditingMode = InkOverlayEditingMode.Ink;

            // Update the state of the Ink and Erase menu items
            miInk.Checked = true;
            miErase.Checked = false;

            // Update the UI
            this.Refresh();
        }

        /// <summary>
        /// Event handler for Mode->Erase menu item
        /// </summary>
        /// <param name="sender">The control that raised the event.</param>
        /// <param name="e">The event argument.</param>
        private void miErase_Click(object sender, System.EventArgs e)
        {
            // Turn on the ink deletion mode
            myInkOverlay.EditingMode = InkOverlayEditingMode.Delete;

            // Update the state of the Ink and Erase menu items
            miInk.Checked = false;
            miErase.Checked = true;

            // Update the UI
            this.Refresh();
        }

        /// <summary>
        /// Event handler for Mode->AutomaticLayoutAnalysis menu item.
        /// </summary>
        /// <param name="sender">The control that raised the event.</param>
        /// <param name="e">The event argument.</param>
        private void miAutomaticLayoutAnalysis_Click(object sender, System.EventArgs e)
        {
            // Toggle the check on the menu item.
            miAutomaticLayoutAnalysis.Checked = !miAutomaticLayoutAnalysis.Checked;
            // Update the window.
            this.Refresh();
        }

        /// <summary>
        /// Event handler for Mode->Rotated Bounding Boxes menu item.
        /// </summary>
        /// <param name="sender">The control that raised the event.</param>
        /// <param name="e">The event argument.</param>
        private void miRotatedBoundingBox_Click(object sender, EventArgs e)
        {
            // Toggle the check on the menu item.
            miRotatedBoundingBoxes.Checked = !miRotatedBoundingBoxes.Checked;

            //roated bounding boxes is checked
            if (miRotatedBoundingBoxes.Checked)
            {
                // Since Rotated Bounding Boxes is checked
                // we don't want to draw the horizontal bounding
                // boxes, so set their values to null
                myWordHorizontalBBoxes = null;
                myDrawingHorizontalBBoxes = null;
                myBulletHorizontalBBoxes = null;
                myLineHorizontalBBoxes = null;
                myParagraphHorizontalBBoxes = null;

                //Call RoatedBoundingBoxChecked helper method to 
                //ensure the rotated bounding boxes values are
                //up to date
                CalculateRotatedBoundingBoxes();

            }
            //rotated bounding boxes is not checked.
            else
            {
                // Since Rotated Bounding Boxes is NOT checked
                // we don't want to draw the rotated bounding
                // boxes, so set their values to null
                myWordRotatedBoundingBoxes = null;
                myDrawingRotatedBoundingBoxes = null;
                myBulletRotatedBoundingBoxes = null;
                myLineRotatedBoundingBoxes = null;
                myParagraphRotatedBoundingBoxes = null;

                // Call RoatedBoundingBoxUnChecked helper method to 
                // ensure the rotated bounding boxes values are
                // up to date
                CalculateHorizontalBoundingBoxes();

            }

            // Update the window.
            this.Refresh();
        }
        #endregion

        #region Constructors and Destructors

        /// <summary>
        /// Create and initialize the subcomponents of this form.
        /// </summary>
        public BasicInkAnalysisSampleForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();

                //  Dispose the ink overlay's resources
                if (myInkOverlay != null)
                {
                    myInkOverlay.Dispose();
                }

                //  Dispose the Analyzer's resources
                if (myInkAnalyzer != null)
                {
                    // Only need to dispose of the InkAnalyzer.  All other
                    // objects will be automatically disposed.
                    myInkAnalyzer.Dispose();
                }

                // Dispose the Pen used for drawing
                if (penBox != null)
                {
                    penBox.Dispose();
                }
            }
            base.Dispose(disposing);
        }
        #endregion

    }
}
