/*
------------------------------------------------------------------------------------
This file is part of OpenModelica.

Copyright (c) 1998-2006, Link�pings universitet,
Department of Computer and Information Science, PELAB
See also: www.ida.liu.se/projects/OpenModelica

All rights reserved.

(The new BSD license, see also
http://www.opensource.org/licenses/bsd-license.php)


Redistribution and use in source and binary forms, with or without
modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    
	* Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    * Neither the name of Link�pings universitet nor the names of its contributors
      may be used to endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

For more information about the Qt-library visit TrollTech:s webpage regarding
licence: http://www.trolltech.com/products/qt/licensing.html

------------------------------------------------------------------------------------
*/

// FILE/CLASS ADDED 2005-11-09 /AF

/*! 
 * \file stripstring.h
 * \author Anders Fernstr�m
 */

#ifndef STRIPSTRING_H
#define STRIPSTRING_H

//STD Headers
#include <string>

//QT Headers
#include <QtCore/QStringList>


typedef pair<string,string> rule_t;
typedef vector<rule_t> rules_t;


using namespace std;


/*! 
	* \class CellStyle
	* \author Anders Fernstr�m
	* \date 2005-11-09
	* 
	* \brief A class that remove/add some stuff in the content strings taken 
	* from mathematica notebooks.
	*/
class StripString
{
public:
	StripString(){}
	~StripString(){}

	static string stripNBString( string str )
	{
        string::size_type pos = 0;
        while((pos = str.find("\\", pos)) != string::npos)
        {
            switch(str[pos+1])
            {
            case 'n':
				// replace '\n' with html newline (<br>)
                str.replace(pos, 2, "<br>");
                break;
            case 't':
				// replace '\t' with 4 html blankspace
				str.replace( pos, 2, "&nbsp;&nbsp;&nbsp;&nbsp;" );
				break;
			case 'r':
				// remove '\r' from the text
				str.erase( pos, 2 );	
				break;
            case '"':
            case '[':
				// remove '\"' and '\[' from the text.
				// also repace mathematica notebook symbols for the letter �,�,�
				if( str.find( "[ODoubleDot]", pos ) == pos+1 )
					str.replace( pos, 13, "�" );
				else if( str.find( "[CapitalODoubleDot]", pos ) == pos+1 )
					str.replace( pos, 20, "�" );
				else if( str.find( "[ADoubleDot]", pos ) == pos+1 )
					str.replace( pos, 13, "�" );
				else if( str.find( "[CapitalADoubleDot]", pos ) == pos+1 )
					str.replace( pos, 20, "�" );
				else if( str.find( "[ARing]", pos ) == pos+1 )
					str.replace( pos, 8, "�" );
				else if( str.find( "[CapitalARing]", pos ) == pos+1 )
					str.replace( pos, 15, "�" );
				else if( str.find( "[LeftRightArrow]", pos ) == pos+1 ) // NOT CORRECT SYMBOL
					str.replace( pos, 17, "<->" );
				else if( str.find( "[Bullet]", pos ) == pos+1 )
					str.replace( pos, 9, "*" );
				else if( str.find( "[Dash]", pos ) == pos+1 )
					str.replace( pos, 7, "-" );
				else if( str.find( "[CloseCurlyQuote]", pos ) == pos+1 )
					str.replace( pos, 18, "'" );
				else if( str.find( "[UnderBracket]", pos ) == pos+1 ) // NOT CORRECT SYMBOL
					str.replace( pos, 15, "_" );
				else if( str.find( "[LeftDoubleBracket]", pos ) == pos+1 ) // NOT CORRECT SYMBOL
					str.replace( pos, 20, "[[" );
				else if( str.find( "[RightDoubleBracket]", pos ) == pos+1 ) // NOT CORRECT SYMBOL
					str.replace( pos, 21, "]]" );
				else if( str.find( "[Equal]", pos ) == pos+1 )
					str.replace( pos, 8, "=" );
				else if( str.find( "[SkeletonIndicator]", pos) == pos+1 )
					str.replace( pos, 20, "-" );
				else
					str.erase( pos, 1 );
				break;
            default:
                pos += 1;
                break;
            }
		}

		return str;
	}

	static string stripSimulationData( string str )
	{
		string::size_type pos = 0;
		if( string::npos != str.find( "\"<SimulationData", pos ))
		{
			//return "[Strange Mathematica code of SimulationData removed here]";
			return "";
		}

		return str;
	}

	/*! 
	 * \author Anders Fernstr�m
	 * \date 2005-12-06
	 * \date 2005-12-08 (update)
	 *
	 * \brief Apply rules to a text
	 *
	 * 2005-12-08 AF, Implemented large part of the function
	 */ 
	static string applyRulesToText( string str, rules_t rules )
	{
		// loop through all rules and apply them
		for( rules_t::iterator r_iter = rules.begin(); r_iter != rules.end(); r_iter++ )
		{
			// FONT FAMILY
			if( (*r_iter).first == "FontFamily" )
			{
				// replace 'Courier' with 'Courier New'
				string family = (*r_iter).second;
				if( family == "Courier" )
					family = "Courier New";

				// insert font family
				family = "font-family:" + family + "; ";
				string::size_type index = getSpanIndex( str );
				str.insert( index, family );
			}
			// FONT SIZE
			else if( (*r_iter).first == "FontSize" )
			{
				string sizept = (*r_iter).second;
				string::size_type pos = 0;
				pos = sizept.find("`", pos);
				if( pos != string::npos)
					sizept.erase( pos, 1 );

				string size = "font-size:" + sizept + "pt; ";
				string::size_type index = getSpanIndex( str );
				str.insert( index, size );
			}
			// FONT WEIGHT
			else if( (*r_iter).first == "FontWeight" )
			{
				string::size_type index = getSpanIndex( str );

				// BOLD
				if( (*r_iter).second == "Bold" )
					str.insert( index, "font-weight:600; " );
				// OTHER
				else
					cout << "[UNKNOWN] Rule::FontWeight::Value: " << (*r_iter).second.c_str() << endl;
			}
			// FONT SLANT
			else if( (*r_iter).first == "FontSlant" )
			{
				string::size_type index = getSpanIndex( str );

				// ITALIC
				if( (*r_iter).second == "Italic" )
					str.insert( index, "font-style:italic; " );
				// OTHER
				else
					cout << "[UNKNOWN] Rule::FontSlant::Value: " << (*r_iter).second.c_str() << endl;
			}
			// FONT COLOR
			else if( (*r_iter).first == "FontColor" )
			{
				QString color( (*r_iter).second.c_str() );
				QStringList colorlist = color.split( ":" );

				if( colorlist.size() == 3 )
				{
					// red
					bool okRed;
					QString tmp = colorlist.at(0);
					tmp.remove( "`" );
					double red = 255 * tmp.toDouble( &okRed );

					// green
					bool okGreen;
					tmp = colorlist.at(1);
					tmp.remove( "`" );
					double green = 255 * tmp.toDouble( &okGreen );

					// blue
					bool okBlue;
					tmp = colorlist.at(2);
					tmp.remove( "`" );
					double blue = 255 * tmp.toDouble( &okBlue );

					if( okRed && okGreen && okBlue )
					{
						// red
						tmp = QString::number( (int)red, 16 );
						if( tmp.length() == 1 )
							tmp = "0" + tmp;
						color = tmp;

						// green
						tmp = QString::number( (int)green, 16 );
						if( tmp.length() == 1 )
							tmp = "0" + tmp;
						color += tmp;
						
						// blue
						tmp = QString::number( (int)blue, 16 );
						if( tmp.length() == 1 )
							tmp = "0" + tmp;
						color += tmp;
						
						// set the color
						color = "color:#" + color + "; ";
						string::size_type index = getSpanIndex( str );
						str.insert( index, color.toStdString() );
					}
					else
						cout << "[UNKNOWN] StyleBox::Rule::RBGColor::Value: " << (*r_iter).second.c_str() << endl;
				}
				else
					cout << "[UNKNOWN] StyleBox::Rule::RBGColor::Value: " << (*r_iter).second.c_str() << endl;
			}
			// FONT VARIATIONS
			else if( (*r_iter).first == "FontVariations" )
			{
				// IGNORE: Don't realy know what to do here...
			}
			// OTHER / MISC
			else
				cout << "[UNKNOWN] StyleBox::Rule: " << (*r_iter).first.c_str() << " - " << (*r_iter).second.c_str() << endl;
		}

		return str;
	}

	/*! 
	 * \author Anders Fernstr�m
	 * \date 2005-12-08
	 *
	 * \brief Apply a html <span style=""> tag to the text, and 
	 * return the index of the correct position inside the style
	 */ 
	static string::size_type getSpanIndex( string &str )
	{
		if( str.find( "<span style=\"", 0) == string::npos )
		{
			// no span tag, add one
			string tmp = "<span style=\" \">" + str + "</span>";
			str = tmp;
		}
	
		// go to the end of the style="" part
		return str.find( "\">", 0 );
	}

	/*! 
	 * \author Anders Fernstr�m
	 * \date 2006-02-10
	 *
	 * \brief Add # symbol to filenames for links.
	 *
	 * HTML link look like: Dir/filename.html#ChaperSeven
     * The notebook parser returns: Dir/filename.htmlChapterSeven
     * have to insert # symbol
	 */ 
	static string fixFilename( string filename )
	{
		string::size_type index = filename.find( ".nb" );

		// didn't found '.nb', assume that it is an internal doc link
		// and add '#' first in the filename
		if( index == string::npos )
		{
			filename = string("#") + filename;
			return filename;
		}

		// found .nb
		if( index + 4 < filename.length() )
		{
			// have some text behide the fileending. Add # symbol
			filename.insert( index + 3, "#" );
			return filename;
		}
		else
			return filename;
	}

};

#endif
