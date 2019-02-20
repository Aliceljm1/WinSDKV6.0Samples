SimpleProviderUnmanaged

This sample application creates a simple custom control that supports UI Automation.

The control is a simple button-like control that supports InvokePattern. Clicking the 
button causes it to change color. You can also tab to the button and click it by pressing
the spacebar.
 
To test the functionality of InvokePattern, you can use the UISpy tool. Click on the control
in the UI Automation raw view or control view and then select Control Patterns from the
View menu. In the Control Patterns dialog box, you can call the InvokePattern::Invoke method.
