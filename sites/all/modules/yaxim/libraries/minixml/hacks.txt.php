<?php

// $Id: hacks.txt.php,v 1.2 2006/08/15 17:13:12 jaza Exp $

/**
 * @file
 * Documents all modifications made to the original MiniXML library, for use
 * with the Drupal Import / Export API module. There is no code in this file,
 * just text within comments. This is a PHP file as a security precaution only.
 */

/*   MODIFICATIONS MADE TO MINIXML:

0    Misc

0.1  Added this file ('hacks.txt.php') to the main directory.
0.2  Removed the 'docs' subdirectory, and all files within it.

1    minixml.inc.php

1.1  Added CVS ID tag (line 3).
1.2  Added doxygen file descriptor (lines 5-8).
1.3  Changed the constant MINIXML_AUTOESCAPE_ENTITIES from 1 to 0.
1.4  Added the constant MINIXML_INDENT_SIZE, defaulting to 2 (line 82).
1.5  Changed the constant MINIXML_FROMFILECACHEDIR to use the Drupal variable
     'file_directory_temp', if the variable_get() function is available (lines
     97-101).

2    doc.inc.php

2.1  Added CVS ID tag (line 3).
2.2  Added doxygen file descriptor (lines 5-8).
2.3  Removed the padding before the first element outputted by the _toString()
     method (line 758).
2.4  Removed the class 'MiniXML', since it is a redundant alias for the class
     'MiniXMLDoc' (lines 870-895).

3    element.inc.php

3.1  Added CVS ID tag (line 3).
3.2  Added doxygen file descriptor (lines 5-8).
3.3  Removed the space that was getting put after XML start tags (line 1127).
3.4  Removed the space that was getting put before XML end tags (line 1163).

4    node.inc.php

4.1  Added CVS ID tag (line 3).
4.2  Added doxygen file descriptor (lines 5-8).

5    treecomp.inc.php

5.1  Added CVS ID tag (line 3).
5.2  Added doxygen file descriptor (lines 5-8).
5.3  Changed the _spaceStr() method to use indenting spaces according to the
     new constant MINIXML_INDENT_SIZE, and to start looping from 1 instead of 0
     (lines 162-164).
*/
