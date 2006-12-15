package ome.io.nio.utests;

import org.testng.annotations.*;
import junit.framework.TestCase;

import ome.io.nio.DimensionsOutOfBoundsException;
import ome.io.nio.PixelBuffer;
import ome.io.nio.PixelsService;
import ome.model.core.Pixels;
import ome.model.enums.PixelsType;

public class HugePixelBufferUnitTest extends TestCase {
    private ome.model.core.Pixels pixels;

    private PixelBuffer pixelBuffer;

    private static final int planeSize = 1024 * 1024 * 2;

    private static final int stackSize = planeSize * 64;

    private static final int timepointSize = stackSize * 3;

    @Override
    @Configuration(beforeTestMethod = true)
    protected void setUp() {
        pixels = new Pixels();

        pixels.setId(1L);
        pixels.setSizeX(1024);
        pixels.setSizeY(1024);
        pixels.setSizeZ(64);
        pixels.setSizeC(3);
        pixels.setSizeT(50);

        PixelsType type = new PixelsType();
        type.setValue("uint16");
        pixels.setPixelsType(type);

        PixelsService service = new PixelsService(PixelsService.ROOT_DEFAULT);
        pixelBuffer = service.getPixelBuffer(pixels);
    }

    @Test
    public void testGetPlaneSize() {
        assertEquals(pixelBuffer.getPlaneSize().intValue(), planeSize);
    }

    @Test
    public void testGetStackSize() {
        assertEquals(pixelBuffer.getStackSize().intValue(), stackSize);
    }

    @Test
    public void testGetTimepointSize() {
        assertEquals(pixelBuffer.getTimepointSize().intValue(), timepointSize);
    }

    @Test
    public void testGetInitialPlaneOffset()
            throws DimensionsOutOfBoundsException {
        assertEquals(pixelBuffer.getPlaneOffset(0, 0, 0).longValue(), 0L);
    }

    @Test
    public void testGetPlaneOffset1() throws DimensionsOutOfBoundsException {
        long offset = (long) timepointSize * 25 + (long) planeSize * 25;
        assertEquals(pixelBuffer.getPlaneOffset(25, 0, 25).longValue(), offset);
    }

    @Test
    public void testGetPlaneOffset2() throws DimensionsOutOfBoundsException {
        long offset = (long) timepointSize * 25 + (long) stackSize * 1
                + (long) planeSize * 25;
        assertEquals(pixelBuffer.getPlaneOffset(25, 1, 25).longValue(), offset);
    }
}
