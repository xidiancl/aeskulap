/*
 *
 *  Copyright (C) 2002-2005, OFFIS
 *
 *  This software and supporting documentation were developed by
 *
 *    Kuratorium OFFIS e.V.
 *    Healthcare Information and Communication Systems
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
 *  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
 *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
 *  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
 *  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
 *
 *  Module:  dcmimage
 *
 *  Author:  Marco Eichelberg
 *
 *  Purpose: class DcmQuantColorMapping
 *
 *  Last Update:      $Author$
 *  Update Date:      $Date$
 *  CVS/RCS Revision: $Revision$
 *  Status:           $State$
 *
 *  CVS/RCS Log at end of file
 *
 */


#ifndef DIQTCMAP_H
#define DIQTCMAP_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmimage/diqttype.h"  /* for DcmQuantComponent */
#include "dcmtk/dcmimgle/dcmimage.h"  /* gcc 3.4 needs this */
#include "dcmtk/dcmimage/diqtstab.h"  /* gcc 3.4 needs this */
#include "dcmtk/dcmimage/diqtpix.h"   /* gcc 3.4 needs this */
#include "dcmtk/dcmimage/diqthash.h"  /* gcc 3.4 needs this */
#include "dcmtk/dcmimage/diqtctab.h"  /* gcc 3.4 needs this */

class DicomImage;
class DcmQuantColorHashTable;
class DcmQuantColorTable;
class DcmQuantPixel;
class DcmQuantScaleTable;


/** template class that maps a color image into a palette color image
 *  with given color palette.  The two template parameters define the
 *  error diffusion class used to implement e.g. Floyd-Steinberg error
 *  diffusion and the output type of the color index values, which may be
 *  8 bit or 16 bit unsigned integers.
 */
template <class T1, class T2>
class DcmQuantColorMapping
{
public:

  /** converts a single frame of a color image into a palette color image.
   *  @param sourceImage color image
   *  @param frameNumber number of frame (in sourceImage) that is converted
   *  @param maxval maximum pixel value to which all color samples
   *    were down-sampled during computation of the histogram on which
   *    the color LUT is based.  This value is required to make sure that the
   *    hash table doesn't get too large.
   *  @param cht color hash table.  This table is passed by the caller since
   *    the same hash table can be used if multiple frames are converted.
   *    Initially (i.e. when called for the first frame) the hash table is empty.
   *  @param colormap color LUT to which the color image is mapped.
   *  @param fs error diffusion object, e.g. an instance of class DcmQuantIdent
   *    or class DcmQuantFloydSteinberg, depending on the template instantiation.
   *  @param tp pointer to an array to which the palette color image data
   *    is written.  The array must be large enough to store sourceImage.getWidth()
   *    times sourceImage.getHeight() values of type T2.
   */
  static void create(
    DicomImage& sourceImage,
    unsigned long frameNumber,
    unsigned long maxval,
    DcmQuantColorHashTable& cht,
    DcmQuantColorTable& colormap,
    T1& fs,
    T2 *tp)
  {
    unsigned long cols = sourceImage.getWidth();
    unsigned long rows = sourceImage.getHeight();
    const int bits = sizeof(DcmQuantComponent)*8;
    DcmQuantPixel px;
    long limitcol;
    long col; // must be signed!
    long maxval_l = OFstatic_cast(long, maxval);
    register int ind;
    const DcmQuantComponent *currentpixel;
    register DcmQuantComponent cr, cg, cb;

    // create scale table
    DcmQuantScaleTable scaletable;
    scaletable.createTable(OFstatic_cast(DcmQuantComponent, -1), maxval);

    const void *data = sourceImage.getOutputData(bits, frameNumber, 0);
    if (data)
    {
      const DcmQuantComponent *cp = OFstatic_cast(const DcmQuantComponent *, data);
      for (unsigned long row = 0; row < rows; ++row)
      {
        fs.startRow(col, limitcol);
        do
        {
        	currentpixel = cp + col + col + col;
        	cr = *currentpixel++;
        	cg = *currentpixel++;
        	cb = *currentpixel;
            px.scale(cr, cg, cb, scaletable);

            fs.adjust(px, col, maxval_l);

            // Check hash table to see if we have already matched this color.
            ind = cht.lookup(px);
            if (ind < 0)
            {
              ind = colormap.computeIndex(px);
              cht.add(px, ind);
            }

            fs.propagate(px, colormap.getPixel(ind), col);
            tp[col] = OFstatic_cast(T2, ind);
            fs.nextCol(col);
        } while ( col != limitcol );
        fs.finishRow();
        cp += (cols * 3); // advance source pointer by one row
        tp += cols;  // advance target pointer by one row
      } // for all rows
    } // if (data)
  }
};


#endif


/*
 * CVS/RCS Log:
 * $Log$
 * Revision 1.1  2007/04/24 09:53:24  braindead
 * - updated DCMTK to version 3.5.4
 * - merged Gianluca's WIN32 changes
 *
 * Revision 1.1.1.1  2006/07/19 09:16:44  pipelka
 * - imported dcmtk354 sources
 *
 *
 * Revision 1.4  2005/12/08 16:01:44  meichel
 * Changed include path schema for all DCMTK header files
 *
 * Revision 1.3  2004/04/21 10:00:31  meichel
 * Minor modifications for compilation with gcc 3.4.0
 *
 * Revision 1.2  2003/12/23 12:14:38  joergr
 * Adapted type casts to new-style typecast operators defined in ofcast.h.
 * Updated copyright header.
 *
 * Revision 1.1  2002/01/25 13:32:04  meichel
 * Initial release of new color quantization classes and
 *   the dcmquant tool in module dcmimage.
 *
 *
 */
