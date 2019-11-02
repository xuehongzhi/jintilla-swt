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
 * Describes a range, a String of text, and its capacity (length).
 * @author Valentin Valchev <v_valchev at prosyst.bg>
 * @author Nathan Sweet <misc at n4te.com>
 */
public class TextRange {
	public TextRange () {
	}

	public TextRange (int start, int end) {
		this.start = start;
		this.end = end;
	}

	public TextRange (int start, int end, int capacity) {
		this.start = start;
		this.end = end;
		this.capacity = capacity;
	}

	public String text; // Populated with the text. capacity must be set if end is -1.
	public int capacity; // Necessary to allocate enough memory on the C side to store the whole document if end is -1.
								// Otherwise it is not used.
	public int start;
	public int end; // Can be -1 to return until the end of the document.
}
