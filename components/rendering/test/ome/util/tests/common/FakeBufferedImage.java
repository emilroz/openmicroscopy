/*
 * ome.util.tests.common.FakeBufferedImage
 *
 *   Copyright 2006 University of Dundee. All rights reserved.
 *   Use is subject to license terms supplied in LICENSE.txt
 */

package ome.util.tests.common;

// Java imports
import java.awt.image.BufferedImage;

// Third-party libraries

// Application-internal dependencies

/**
 * Fake object. A {@link BufferedImage} of <code>2x2</code> pixels of
 * <code>TYPE_BYTE_GRAY</code>.
 * 
 * @author Jean-Marie Burel &nbsp;&nbsp;&nbsp;&nbsp; <a
 *         href="mailto:j.burel@dundee.ac.uk">j.burel@dundee.ac.uk</a>
 * @author <br>
 *         Andrea Falconi &nbsp;&nbsp;&nbsp;&nbsp; <a
 *         href="mailto:a.falconi@dundee.ac.uk"> a.falconi@dundee.ac.uk</a>
 * @version 2.2 <small> (<b>Internal version:</b> $Revision$ $Date:
 *          2005/06/25 18:09:17 $) </small>
 * @since OME2.2
 */
public class FakeBufferedImage extends BufferedImage {

    public FakeBufferedImage() {
        super(2, 2, TYPE_BYTE_GRAY);
    }

}
