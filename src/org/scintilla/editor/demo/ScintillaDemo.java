/*
 * The Jintilla Project - This is a Java SWT port of Scintilla, a free source code editing component.
 * Copyright (C) 2005  Nathan Sweet
 * 
 * http://sourceforge.net/projects/jintilla
 * http://www.scintilla.org/
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

package org.scintilla.editor.demo;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;
import org.scintilla.editor.CText;
import org.scintilla.editor.NotificationEvent;
import org.scintilla.editor.NotificationListener;
import org.scintilla.editor.Scintilla;
import org.scintilla.editor.ScintillaConstants;

/**
 * Simple demo of the Scintilla control.
 * @author Nathan Sweet <misc at n4te.com>
 */
public class ScintillaDemo {

	public static void main (String args[]) {
		Display display = new Display();

		Shell shell = new Shell(display);
		shell.setSize(new org.eclipse.swt.graphics.Point(580, 354));
		shell.setText("Scintilla Test");

		GridLayout gridLayout = new GridLayout();
		shell.setLayout(gridLayout);
		gridLayout.marginWidth = 0;

		final Scintilla scintilla = new Scintilla(shell, SWT.NONE);
		scintilla.setFocus();
		scintilla.setBackground(org.eclipse.swt.widgets.Display.getDefault().getSystemColor(org.eclipse.swt.SWT.COLOR_RED));
		GridData gridData1 = new GridData();
		scintilla.setLayoutData(gridData1);
		gridData1.grabExcessHorizontalSpace = true;
		gridData1.grabExcessVerticalSpace = true;
		gridData1.horizontalAlignment = GridData.FILL;
		gridData1.verticalAlignment = GridData.FILL;

		scintilla.addNotificationListener(new NotificationListener() {
			public void handleNotificationEvent (NotificationEvent event) {
				if (event.code != ScintillaConstants.SCN_MODIFIED) return;
				StringBuffer types = new StringBuffer();
				if (event.isModificationType(ScintillaConstants.SC_MOD_INSERTTEXT)) types.append("SC_MOD_INSERTTEXT ");
				if (event.isModificationType(ScintillaConstants.SC_MOD_DELETETEXT)) types.append("SC_MOD_DELETETEXT ");
				if (event.isModificationType(ScintillaConstants.SC_MOD_CHANGESTYLE)) types.append("SC_MOD_CHANGESTYLE ");
				if (event.isModificationType(ScintillaConstants.SC_MOD_CHANGEFOLD)) types.append("SC_MOD_CHANGEFOLD ");
				if (event.isModificationType(ScintillaConstants.SC_PERFORMED_USER)) types.append("SC_PERFORMED_USER ");
				if (event.isModificationType(ScintillaConstants.SC_PERFORMED_UNDO)) types.append("SC_PERFORMED_UNDO ");
				if (event.isModificationType(ScintillaConstants.SC_PERFORMED_REDO)) types.append("SC_PERFORMED_REDO ");
				if (event.isModificationType(ScintillaConstants.SC_LASTSTEPINUNDOREDO)) types.append("SC_LASTSTEPINUNDOREDO ");
				if (event.isModificationType(ScintillaConstants.SC_MOD_CHANGEMARKER)) types.append("SC_MOD_CHANGEMARKER ");
				if (event.isModificationType(ScintillaConstants.SC_MOD_BEFOREINSERT)) types.append("SC_MOD_BEFOREINSERT ");
				if (event.isModificationType(ScintillaConstants.SC_MOD_BEFOREDELETE)) types.append("SC_MOD_BEFOREDELETE ");
				if (event.isModificationType(ScintillaConstants.SC_MODEVENTMASKALL)) types.append("SC_MODEVENTMASKALL ");
				System.out.println(event.text + " (" + types + ")");
			}
		});

		Button button = new Button(shell, SWT.FLAT);
		button.setText("Test");
		GridData gridData2 = new GridData();
		button.setLayoutData(gridData2);
		gridData2.horizontalAlignment = GridData.CENTER;
		gridData2.verticalAlignment = GridData.CENTER;

		button.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected (SelectionEvent evt) {
				// Play with the control here.
				CText ctext = new CText(400);
				int charCount = scintilla.getText(400, ctext);
				System.out.println(scintilla.getCurrentPos() + " " + ctext.text);
			}
		});

		shell.layout();
		shell.open();

		while (!shell.isDisposed()) {
			if (!display.readAndDispatch()) display.sleep();
		}
		display.dispose();
	}

}
