// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// 
//  File: InkAnalysisScannedPaperForm.cs 
//
//  This sample application demonstrates the following features of the 
//  Ink Analysis API and the Windows Forms Inking Controls:
//  * Loading a scanned paper form (as an image in .png format)
//  * Using an InkOverlay to gather and display Ink over the scanned form
//  * Using an InkAnalyzer to interpret handwriting results
//  * Generating AnalysisHintNodes to improve handwriting results
//  * Populating TextBoxes from AnalysisHintNodes for a basic correction 
//    experience
// 
//-------------------------------------------------------------------------- 

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Drawing.Drawing2D;
using Microsoft.Ink;

namespace Microsoft.Samples.TabletPC.InkAnalysisScannedPaperForm
{
    /// <summary>
    /// The main window for the InkAnalysis Scanned Paper Form Demo derived form Windows.Forms.Form
    /// </summary>
    public class ScannedPaperForm : System.Windows.Forms.Form
    {

        // This collection of TextBoxes will be updated with analysis 
        // results after analysis is completed.
        private ArrayList textBoxes;

        // These strings will be used with AnalysisHintNodes to describe
        // the expected text for each field in the paper form.
        private static string[] factoidStrings =
        {
            // string for the "For:" Field
            // Indicates that we expect a full name
            "(!IS_PERSONALNAME_FULLNAME)",
            
            // string for the "Date:" Field
            // Indicates that we expect a complete date
            "(!IS_DATE_FULLDATE)",
            
            // string for the "Time:" Field
            // Indicates that we expect a complete time
            "(!IS_TIME_FULLTIME)",
            
            // string for the "From:" Field
            // Indicates that we expect a full name
            "(!IS_PERSONALNAME_FULLNAME)",
            
            // string for the "Phone Number:" AnalysisHintNode
            // Indicates that we expect a phone number, 
            // including the area code
            "(!IS_TELEPHONENUMBER_FULLNUMBER)",
            
            // string for the "Message:" AnalysisHintNode
            // We don't wat to specify any extra information,
            // since this field could contain anything, even
            // drawings
            "",
            
            // string for the "Taken by:" AnalysisHintNode
            // Indicates that we expect a full name
            "(!IS_PERSONALNAME_FULLNAME)"
        };


        // Each Rectangle in the Rectangles array will be used for the bounds of an 
        // AnalysisHintNode.  
        private static Rectangle[] rects = 
        {            
            // Create a Rectangle for the "For:" field
            new Rectangle(3040, 2507, 12591, 662),
            
            // Create a Rectangle for the "Date:" field
            new Rectangle(3040, 3486, 6500, 662),
            
            // Create a Rectangle for the "Time:" field
            new Rectangle(12303, 3486, 3328, 662),
            
            // Create a Rectangle for the "From:" field
            new Rectangle(3040, 4440, 12591, 662),
            
            // Rectangle a Rectangle for the "Phone Number:" field
            new Rectangle(7143, 5440, 8488, 662),
            
            // Create a Rectangle for the "Message:" field
            new Rectangle(800, 8070, 14831, 6085),
            
            // Create a Rectangle for the "Taken by:" field
            new Rectangle(4910, 15252, 10721, 662)
        };

        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.Container components = null;

        // The UI for this application consists of a TabControl
        // with two Tab Pages - one for inking and one for converted
        // text.  
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage inkForm;
        private System.Windows.Forms.TabPage convertedTextForm;

        // The inkForm contains a scrollable inking surface (implemented
        // via nested panels) and buttons for hiding and showing the bounds 
        // of the AnalysisHintNodes, clearing the ink on the form, and invoking analysis.
        private System.Windows.Forms.Panel inkFormTopLevelPanel;
        private System.Windows.Forms.Panel inkFormScrollPanel;
        private System.Windows.Forms.Panel inkFormInkingPanel;
        private System.Windows.Forms.Panel inkFormButtonPanel;
        private System.Windows.Forms.Button analyzeButton;
        private System.Windows.Forms.Button clearButton;
        private System.Windows.Forms.CheckBox hintCheckBox;

        // The textForm consists of a single panel, to which TextBoxes
        // will be added via the InitTextBox method.
        private System.Windows.Forms.Panel convertedTextPanel;

        // The InkOverlay collects and displays ink.
        private InkOverlay myInkOverlay;

        // The InkAnalyzer object is the core of the InkAnalysis API.
        private InkAnalyzer analyzer;

        public ScannedPaperForm()
        {
            // Initialize UI Components
            InitializeComponent();

            // Set the Border style to be non-resizable
            this.FormBorderStyle = FormBorderStyle.FixedSingle;

            // Disable the maximize box
            this.MaximizeBox = false;

            // Construct an InkOverlay attached to the inkForm's inking panel
            myInkOverlay = new InkOverlay(this.inkFormInkingPanel);

            // Create an InkAnalyzer, passing the InkOverlay's Ink Object.
            analyzer = new InkAnalyzer(myInkOverlay.Ink, this);

            // Add an event handler that adds Strokes to the InkAnalyzer as they
            // are collected.
            myInkOverlay.Stroke += new InkCollectorStrokeEventHandler(MyInkOverlayStroke);

            // Enable the InkOverlay to begin collection.
            myInkOverlay.Enabled = true;

            // Add a paint handler to the inking panel to draw the AnalysisHintNodes
            // bounds if the "Show Analysis Hint Bounding Boxes" CheckBox is checked.
            this.inkFormInkingPanel.Paint += new PaintEventHandler(InkFormInkingPanelPaint);

            // Create the AnalysisHintNode that will be used to give the InkAnalyzer
            // additional information about how to interpret strokes.  Also, initialize the 
            // TextBoxes where the results from each AnalysisHintNode will be stored.
            this.InitHints();
        }

        /// <summary>
        /// When a stroke is collected, add it to the InkAnalyzer. 
        /// After a Stroke has been added, it will be analyzed 
        /// the next time BackgroundAnalyze or Analyze is called.
        /// </summary>
        private void MyInkOverlayStroke(object sender, InkCollectorStrokeEventArgs e)
        {
            analyzer.AddStroke(e.Stroke);
        }

        /// <summary>
        /// Create AnalysisHintNodes for each area of the scanned 
        /// paper form.
        /// </summary>
        private void InitHints()
        {
            // Instantiate the collection of TextBoxes.
            this.textBoxes = new ArrayList();

            // For each Rectangle in Rectangles
            for (int i = 0; i < rects.Length; i++)
            {

                Rectangle rectangle = rects[i];

                // Create an AnalysisHintNode with the bounds of the Rectangle.  The bounds
                // of an AnalysisHintNode gives clues to the handwriting recognizer about
                // the way Strokes are grouped together.
                AnalysisHintNode hint = this.analyzer.CreateAnalysisHint(rectangle);

                // Set the corresponding Factoid on the AnalysisHintNode.  This gives the 
                // recognizer clues about the meaning of the strokes within the 
                // AnalysisHintNode's region.
                hint.Factoid = factoidStrings[i];

                // Create a corresponding TextBox where the results of the analysis
                // associated with this AnalysisHintNode will be displayed.  Store the reference
                // to the TextBox in the textBoxes Collection.
                this.textBoxes.Add(InitTextBox(hint));
            }
        }

        /// <summary>
        /// Generate a TextBox based on a given AnalysisHintNode
        /// </summary>
        private TextBox InitTextBox(AnalysisHintNode hint)
        {
            // Get the bounds of the AnalysisHintNode
            Rectangle hintBounds = hint.Location.GetBounds();

            // Instantiate a new TextBox
            TextBox textBox = new TextBox();
            textBox.BorderStyle = BorderStyle.None;
            textBox.Multiline = true;

            // Convert the dimension of the AnalysisHintNode's bounding box from
            // Himetric to pixel.
            Rectangle boundsInPixel = this.FromHiMetricToPixel(hintBounds);

            // Set the dimensions of the TextBox to the dimensions of the AnalysisHintNode in pixel
            textBox.Size = new Size(boundsInPixel.Width, boundsInPixel.Height);
            textBox.Location = new Point(boundsInPixel.X, boundsInPixel.Y);

            // Set the background color of the textbox to white         
            textBox.BackColor = Color.White;

            // Set the foreground color of the textbox to black
            textBox.ForeColor = Color.Black;

            // Create a new Font based on the default font, but
            // set the size to 18.
            Font newFont = new Font(textBox.Font.Name, 18);

            // Set the font of the TextBox to the new font
            textBox.Font = newFont;

            // Enable the TextBox to allow user to correct.
            textBox.Enabled = true;

            // Add the TextBox to the InkOverlay in the second 
            // TabPage
            this.convertedTextPanel.Controls.Add(textBox);

            this.tabControl.SelectedIndexChanged += new EventHandler(TabControlSelectedIndexChanged);
            // Return a reference to the TextBox
            return textBox;
        }

        /// <summary>
        /// When the clearButton is clicked, we want to remove all
        /// Strokes from the InkOverlay and InkAnalyzer.
        /// </summary>
        private void ClearButtonClick(object sender, System.EventArgs e)
        {

            if (this.myInkOverlay.Ink.Strokes.Count > 0)
            {
                // Remove the strokes from the InkAnalyzer
                this.analyzer.RemoveStrokes(this.myInkOverlay.Ink.Strokes);

                // Remove the strokes from the InkOverlay
                this.myInkOverlay.Ink.DeleteStrokes(this.myInkOverlay.Ink.Strokes);

                // Invalidate the Inkoverlay to redraw the ink.
                this.inkFormInkingPanel.Invalidate();
            }

            // Clear the text from the TextBoxes.
            for (int i = 0; i < this.textBoxes.Count; i++)
            {
                (this.textBoxes[i] as TextBox).Clear();
            }
        }

        /// <summary>
        /// When the Analyze Button is clicked
        /// switch to the converted text tab item, which will update
        /// the analysis results.
        /// </summary>
        private void AnalyzeButtonClick(object sender, System.EventArgs e)
        {
            // Navigate to the TabPage that contains the TextBoxes.
            // This will trigger the analysis of all unanalyzed ink.
            this.tabControl.SelectedIndex = 1;

        }

        /// <summary>
        /// Update each TextBox if analysis results exist for the corresponding AnalysisHintNode
        /// </summary>
        private void UpdateTextBoxes()
        {
            // Get the AnalysisHintNodes that we previously added to the InkAnalyzer.
            ContextNodeCollection hints = this.analyzer.GetAnalysisHints();

            for (int i = 0; i < hints.Count; i++)
            {
                // Get the recognized string from the AnalysisHintNode
                string analyzedString = ((AnalysisHintNode)hints[i]).GetRecognizedString();

                // If we found a string, set the contents of the TextBox
                // to that string.
                if (analyzedString != null)
                {
                    ((TextBox)this.textBoxes[i]).Text = analyzedString;
                }
            }
        }

        private void HintCheckBoxCheckedChanged(object sender, System.EventArgs e)
        {
            // Invoke a repaint, which will use the state of the CheckBox
            // to determine if the panel should be repainted with or without
            // the AnalysisHintNode Rectangles visible.
            this.inkFormInkingPanel.Invalidate();
        }

        /// <summary>
        /// When the InkingPanel in the InkForm tab is repainted, draw the 
        /// bounds of the AnalysisHintNodes in the document if the hintCheckBox
        /// is checked.
        /// </summary>
        private void InkFormInkingPanelPaint(object sender, PaintEventArgs e)
        {
            // If the hintCheckBox is checked
            if (this.hintCheckBox.Checked)
            {
                // Draw the bounds of the AnalysisHintNodes currently added
                // to the InkAnalyzer.
                this.DrawHintRects(e.Graphics);
            }
        }

        /// <summary>
        /// Draw the bounds of all Rectangles in the rects array.
        /// </summary>
        /// <param name="g">The graphics object where the Rectangles will be drawn</param>
        private void DrawHintRects(Graphics g)
        {
            // Create a red brush for filling the Rectangles with roughly 25% opacity.          
            SolidBrush brush = new SolidBrush(System.Drawing.Color.FromArgb(64, 255, 0, 0));

            // For each Rectangle in Rectangles
            for (int i = 0; i < rects.Length; i++)
            {
                // Convert the coordinates of the Rectangle from HiMetric
                // to Pixel space.
                Rectangle rect = this.FromHiMetricToPixel(rects[i]);

                // Fill the Rectangle with the light red brush.
                g.FillRectangle(brush, rect);
            }
        }

        /// <summary>
        /// Convert the given Rectangle from Ink (HiMetric) to pixel space
        /// </summary>
        private Rectangle FromHiMetricToPixel(Rectangle himetricRect)
        {
            // Get the top left corner of the Rectangle
            Point leftTop = new Point(himetricRect.Left, himetricRect.Top);

            // Get the right bottom corner of the Rectangle
            Point rightBottom = new Point(himetricRect.Right, himetricRect.Bottom);

            // Convert the points from Himetric to pixels
            Graphics g = this.inkFormInkingPanel.CreateGraphics();

            this.myInkOverlay.Renderer.InkSpaceToPixel(g, ref leftTop);
            this.myInkOverlay.Renderer.InkSpaceToPixel(g, ref rightBottom);

            // Create a new Rectangle using the converted Points
            Rectangle rect = new Rectangle(leftTop.X, leftTop.Y, (rightBottom.X - leftTop.X), (rightBottom.Y - leftTop.Y));

            return rect;
        }

        #region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(ScannedPaperForm));
            this.tabControl = new System.Windows.Forms.TabControl();
            this.inkForm = new System.Windows.Forms.TabPage();
            this.inkFormTopLevelPanel = new System.Windows.Forms.Panel();
            this.inkFormButtonPanel = new System.Windows.Forms.Panel();
            this.analyzeButton = new System.Windows.Forms.Button();
            this.clearButton = new System.Windows.Forms.Button();
            this.hintCheckBox = new System.Windows.Forms.CheckBox();
            this.inkFormScrollPanel = new System.Windows.Forms.Panel();
            this.inkFormInkingPanel = new System.Windows.Forms.Panel();
            this.convertedTextForm = new System.Windows.Forms.TabPage();
            this.convertedTextPanel = new System.Windows.Forms.Panel();
            this.tabControl.SuspendLayout();
            this.inkForm.SuspendLayout();
            this.inkFormTopLevelPanel.SuspendLayout();
            this.inkFormButtonPanel.SuspendLayout();
            this.inkFormScrollPanel.SuspendLayout();
            this.convertedTextForm.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.inkForm);
            this.tabControl.Controls.Add(this.convertedTextForm);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(640, 446);
            this.tabControl.TabIndex = 0;
            // 
            // inkForm
            // 
            this.inkForm.Controls.Add(this.inkFormTopLevelPanel);
            this.inkForm.Location = new System.Drawing.Point(4, 22);
            this.inkForm.Name = "inkForm";
            this.inkForm.Size = new System.Drawing.Size(632, 420);
            this.inkForm.TabIndex = 0;
            this.inkForm.Text = "Ink Form";
            // 
            // inkFormTopLevelPanel
            // 
            this.inkFormTopLevelPanel.Controls.Add(this.inkFormButtonPanel);
            this.inkFormTopLevelPanel.Controls.Add(this.inkFormScrollPanel);
            this.inkFormTopLevelPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.inkFormTopLevelPanel.Location = new System.Drawing.Point(0, 0);
            this.inkFormTopLevelPanel.Name = "inkFormTopLevelPanel";
            this.inkFormTopLevelPanel.Size = new System.Drawing.Size(632, 420);
            this.inkFormTopLevelPanel.TabIndex = 0;
            // 
            // inkFormButtonPanel
            // 
            this.inkFormButtonPanel.Controls.Add(this.analyzeButton);
            this.inkFormButtonPanel.Controls.Add(this.clearButton);
            this.inkFormButtonPanel.Controls.Add(this.hintCheckBox);
            this.inkFormButtonPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.inkFormButtonPanel.Location = new System.Drawing.Point(0, 380);
            this.inkFormButtonPanel.Name = "inkFormButtonPanel";
            this.inkFormButtonPanel.Size = new System.Drawing.Size(632, 40);
            this.inkFormButtonPanel.TabIndex = 3;
            // 
            // analyzeButton
            // 
            this.analyzeButton.Location = new System.Drawing.Point(520, 8);
            this.analyzeButton.Name = "analyzeButton";
            this.analyzeButton.Size = new System.Drawing.Size(88, 24);
            this.analyzeButton.TabIndex = 3;
            this.analyzeButton.Text = "Analyze";
            this.analyzeButton.Click += new System.EventHandler(this.AnalyzeButtonClick);
            // 
            // clearButton
            // 
            this.clearButton.Location = new System.Drawing.Point(424, 8);
            this.clearButton.Name = "clearButton";
            this.clearButton.Size = new System.Drawing.Size(88, 24);
            this.clearButton.TabIndex = 2;
            this.clearButton.Text = "Clear";
            this.clearButton.Click += new System.EventHandler(this.ClearButtonClick);
            // 
            // hintCheckBox
            // 
            this.hintCheckBox.Location = new System.Drawing.Point(8, 8);
            this.hintCheckBox.Name = "hintCheckBox";
            this.hintCheckBox.Size = new System.Drawing.Size(208, 16);
            this.hintCheckBox.TabIndex = 1;
            this.hintCheckBox.Text = "Show Analysis Hint Bounding Boxes";
            this.hintCheckBox.CheckedChanged += new System.EventHandler(this.HintCheckBoxCheckedChanged);
            // 
            // inkFormScrollPanel
            // 
            this.inkFormScrollPanel.AutoScroll = true;
            this.inkFormScrollPanel.Controls.Add(this.inkFormInkingPanel);
            this.inkFormScrollPanel.Location = new System.Drawing.Point(0, 0);
            this.inkFormScrollPanel.Name = "inkFormScrollPanel";
            this.inkFormScrollPanel.Size = new System.Drawing.Size(632, 376);
            this.inkFormScrollPanel.TabIndex = 1;
            // 
            // inkFormInkingPanel
            // 
            this.inkFormInkingPanel.AutoScroll = true;
            this.inkFormInkingPanel.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("inkFormInkingPanel.BackgroundImage")));
            this.inkFormInkingPanel.Location = new System.Drawing.Point(0, 0);
            this.inkFormInkingPanel.Name = "inkFormInkingPanel";
            this.inkFormInkingPanel.Size = new System.Drawing.Size(614, 614);
            this.inkFormInkingPanel.TabIndex = 0;
            // 
            // convertedTextForm
            // 
            this.convertedTextForm.AutoScroll = true;
            this.convertedTextForm.AutoScrollMinSize = new System.Drawing.Size(614, 614);
            this.convertedTextForm.Controls.Add(this.convertedTextPanel);
            this.convertedTextForm.Location = new System.Drawing.Point(4, 22);
            this.convertedTextForm.Name = "convertedTextForm";
            this.convertedTextForm.Size = new System.Drawing.Size(632, 420);
            this.convertedTextForm.TabIndex = 1;
            this.convertedTextForm.Text = "Converted Text Form";
            // 
            // convertedTextPanel
            // 
            this.convertedTextPanel.AutoScroll = true;
            this.convertedTextPanel.BackColor = System.Drawing.SystemColors.Window;
            this.convertedTextPanel.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("convertedTextPanel.BackgroundImage")));
            this.convertedTextPanel.Location = new System.Drawing.Point(0, 0);
            this.convertedTextPanel.Name = "convertedTextPanel";
            this.convertedTextPanel.Size = new System.Drawing.Size(614, 614);
            this.convertedTextPanel.TabIndex = 0;
            // 
            // ScannedPaperForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(640, 446);
            this.Controls.Add(this.tabControl);
            this.Name = "ScannedPaperForm";
            this.Text = "InkAnalysis Scanned Paper Form";
            this.tabControl.ResumeLayout(false);
            this.inkForm.ResumeLayout(false);
            this.inkFormTopLevelPanel.ResumeLayout(false);
            this.inkFormButtonPanel.ResumeLayout(false);
            this.inkFormScrollPanel.ResumeLayout(false);
            this.convertedTextForm.ResumeLayout(false);
            this.ResumeLayout(false);

        }
        #endregion

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose(bool disposing)
        {
            // You must call analyzer.Dispose() to clean up
            // resources (both managed and unmanaged).
            analyzer.Dispose();

            if (disposing)
            {
                if (components != null)
                {
                    components.Dispose();
                }
            }
            base.Dispose(disposing);
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.Run(new ScannedPaperForm());
        }

        private void TabControlSelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.tabControl.SelectedIndex == 1)
            {
                // Analyze the strokes that have been added to the InkAnalyzer.
                analyzer.Analyze();

                // Populate the TextBoxes with the results of the analysis.
                this.UpdateTextBoxes();
            }
        }
    }
}
