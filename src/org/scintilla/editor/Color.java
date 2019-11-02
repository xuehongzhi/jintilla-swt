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
 * @author Nathan Sweet <misc at n4te.com>
 */
public class Color {

	static public final int white = Color.rgb(255, 255, 255);
	static public final int lightGray = Color.rgb(240, 240, 240);
	static public final int silver = Color.rgb(192, 192, 192);
	static public final int gray = Color.rgb(128, 128, 128);
	static public final int darkGray = Color.rgb(64, 64, 64);
	static public final int black = Color.rgb(0, 0, 0);
	static public final int red = Color.rgb(255, 0, 0);
	static public final int maroon = Color.rgb(157, 0, 0);
	static public final int magenta = Color.rgb(255, 0, 255);
	static public final int pink = Color.rgb(255, 175, 175);
	static public final int orange = Color.rgb(255, 200, 0);
	static public final int yellow = Color.rgb(255, 255, 0);
	static public final int green = Color.rgb(0, 255, 0);
	static public final int mediumgreen = Color.rgb(0, 164, 82);
	static public final int cyan = Color.rgb(0, 255, 255);
	static public final int purple = Color.rgb(105, 0, 191);
	static public final int blue = Color.rgb(0, 0, 255);
	static public final int cadetblue = Color.rgb(95, 158, 160);
	static public final int darkblue = Color.rgb(0, 0, 128);

	static public final int rgb (int r, int g, int b) {
		return (r & 0xff) | ((g & 0xff) << 8) | ((b & 0xff) << 16);
	}

}
