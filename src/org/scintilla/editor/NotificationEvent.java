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

package org.scintilla.editor;

/**
 * Describes a notification event received from the Scintilla control. Maps to the SCNotification and NotifyHeader structs.
 * Default values must always be provided. Only the values relevant to the notification event are populated through JNI.
 * @author Nathan Sweet <misc at n4te.com>
 */
public class NotificationEvent {
	// Always provided (NMHDR).
	public int hwndFrom = -1;
	public int idFrom = -1;
	public int code = -1;

	// SCN_STYLENEEDED, SCN_MODIFIED, SCN_DWELLSTART, SCN_DWELLEND, SCN_CALLTIPCLICK, SCN_HOTSPOTCLICK, SCN_HOTSPOTDOUBLECLICK
	public int position = -1;

	// SCN_CHARADDED, SCN_KEY
	public int ch = -1;

	// SCN_KEY, SCN_HOTSPOTCLICK, SCN_HOTSPOTDOUBLECLICK
	public int modifiers = -1;

	// SCN_MODIFIED
	public int modificationType = -1;
	public String text = "";
	public int length = -1;
	public int linesAdded = -1;
	public int line = -1;
	public int foldLevelNow = -1;
	public int foldLevelPrev = -1;

	// SCN_MACRORECORD
	public int message = -1;
	public int wParam = -1;
	public int lParam = -1;

	// SCN_MARGINCLICK
	public int margin = -1;

	// SCN_USERLISTSELECTION
	public int listType = -1;

	// SCN_DWELLSTART, SCN_DWELLEND
	public int x = -1;
	public int y = -1;

	/**
	 * Returns true if the NotifyEvent is of the specified modification type constant (SC_PERFORMED_* or SC_MOD_*).
	 */
	public boolean isModificationType (final int type) {
		if (modificationType == -1) return false;
		return (modificationType & type) == type;
	}

}
