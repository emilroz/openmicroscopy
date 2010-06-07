/*
 * ome.formats.utests.ChannelProcessorTest
 *
 *------------------------------------------------------------------------------
 *  Copyright (C) 2006-2010 University of Dundee. All rights reserved.
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *------------------------------------------------------------------------------
 */
package ome.formats.utests;

import junit.framework.TestCase;
import ome.formats.OMEROMetadataStoreClient;
import ome.formats.importer.ImportConfig;
import ome.formats.importer.OMEROWrapper;
import ome.formats.model.BlitzInstanceProvider;
import ome.formats.model.ChannelData;
import ome.formats.model.ChannelProcessor;
import ome.xml.model.enums.*;
import ome.xml.model.primitives.*;
import omero.api.ServiceFactoryPrx;

/**
 * Tests the creation of channel objects.
 * 
 * @author Chris Allan <callan at blackcat dot ca>
 */
public class ChannelProcessorTest 
	extends TestCase
{
	
	/** Reference to the wrapper. */
	private OMEROWrapper wrapper;
	
	/** Reference to the store. */
	private OMEROMetadataStoreClient store;
	
	/** Identifies the index of the filter set. */
	private static final int FILTER_SET_INDEX = 0;
	
	/** Identifies the index of the filter. */
	private static final int FILTER_INDEX = 0;
	
	/** Identifies the index of the light source. */
	private static final int LIGHTSOURCE_INDEX = 0;
	
	/** Identifies the index of the instrument. */
	private static final int INSTRUMENT_INDEX = 0;
	
	/** Identifies the index of the image. */
	private static final int IMAGE_INDEX = 0;

	/** Identifies the index of the channel. */
	private static final int CHANNEL_INDEX = 0;
	
    /**
     * Initializes the components and populates the store.
     */
	@Override
	protected void setUp() throws Exception
	{
		ServiceFactoryPrx sf = new TestServiceFactory();
        wrapper = new OMEROWrapper(new ImportConfig());
        store = new OMEROMetadataStoreClient();
        store.initialize(sf);
        store.setReader(new TestReader());
        store.setEnumerationProvider(new TestEnumerationProvider());
        store.setInstanceProvider(
        		new BlitzInstanceProvider(store.getEnumerationProvider()));
        wrapper.setMetadataStore(store);
        
        // Need to populate at least one pixels and image field.
        store.setImageName("Image", IMAGE_INDEX);
        store.setPixelsSizeX(new PositiveInteger(1), IMAGE_INDEX);
        store.setPixelsSizeC(new PositiveInteger(2), IMAGE_INDEX);
        
        // First Laser, First LightSourceSettings
		store.setLaserID(
				"Laser:0", INSTRUMENT_INDEX, LIGHTSOURCE_INDEX);
		store.setLaserManufacturer("0", INSTRUMENT_INDEX,
				LIGHTSOURCE_INDEX);
		store.setLaserType(LaserType.OTHER, INSTRUMENT_INDEX, LIGHTSOURCE_INDEX);
		store.setChannelLightSourceSettingsID(
				"Laser:0", IMAGE_INDEX, CHANNEL_INDEX);
		store.setChannelLightSourceSettingsAttenuation(
				new PercentFraction(1f), IMAGE_INDEX, CHANNEL_INDEX);
		
		// First Filament , Second LightSourceSettings
		store.setFilamentID(
				"Filament:1", INSTRUMENT_INDEX, LIGHTSOURCE_INDEX);
		store.setFilamentManufacturer("1", INSTRUMENT_INDEX, LIGHTSOURCE_INDEX);
		store.setFilamentType(
        FilamentType.OTHER, INSTRUMENT_INDEX, LIGHTSOURCE_INDEX);
		store.setChannelLightSourceSettingsID(
				"Filament:1", IMAGE_INDEX, CHANNEL_INDEX + 1);
		store.setChannelLightSourceSettingsAttenuation(
				new PercentFraction(1f), IMAGE_INDEX, CHANNEL_INDEX + 1);
		
		// FilterSet
		store.setFilterSetID("FilterSet:0", INSTRUMENT_INDEX,
				FILTER_SET_INDEX);
		store.setFilterSetLotNumber("0", INSTRUMENT_INDEX,
				FILTER_SET_INDEX);
		store.setFilterSetID("FilterSet:1", INSTRUMENT_INDEX,
				FILTER_SET_INDEX + 1);
		store.setFilterSetLotNumber("1", INSTRUMENT_INDEX,
				FILTER_SET_INDEX + 1);
		
		// FilterSet linkages
		store.setChannelFilterSetRef("FilterSet:0", IMAGE_INDEX,
				CHANNEL_INDEX);
		store.setChannelFilterSetRef("FilterSet:1", IMAGE_INDEX,
				CHANNEL_INDEX + 1);
		
		// Filters
		store.setFilterID("Filter:0", INSTRUMENT_INDEX, FILTER_INDEX);
		store.setFilterLotNumber("0", INSTRUMENT_INDEX, FILTER_INDEX);
		store.setFilterID("Filter:1", INSTRUMENT_INDEX, FILTER_INDEX + 1);
		store.setFilterLotNumber("1", INSTRUMENT_INDEX, FILTER_INDEX + 1);
		store.setFilterID("Filter:2", INSTRUMENT_INDEX, FILTER_INDEX + 2);
		store.setFilterLotNumber("2", INSTRUMENT_INDEX, FILTER_INDEX + 2);
		store.setFilterID("Filter:3", INSTRUMENT_INDEX, FILTER_INDEX + 3);
		store.setFilterLotNumber("3", INSTRUMENT_INDEX, FILTER_INDEX + 3);
		store.setFilterID("Filter:4", INSTRUMENT_INDEX, FILTER_INDEX + 4);
		store.setFilterLotNumber("4", INSTRUMENT_INDEX, FILTER_INDEX + 4);
		store.setFilterID("Filter:5", INSTRUMENT_INDEX, FILTER_INDEX + 5);
		store.setFilterLotNumber("5", INSTRUMENT_INDEX, FILTER_INDEX + 5);
		store.setFilterID("Filter:6", INSTRUMENT_INDEX, FILTER_INDEX + 6);
		store.setFilterLotNumber("6", INSTRUMENT_INDEX, FILTER_INDEX + 6);
		store.setFilterID("Filter:7", INSTRUMENT_INDEX, FILTER_INDEX + 7);
		store.setFilterLotNumber("7", INSTRUMENT_INDEX, FILTER_INDEX + 7);
		
		// Filter linkages
		store.setFilterSetEmissionFilterRef("Filter:0", INSTRUMENT_INDEX,
				FILTER_SET_INDEX, FILTER_INDEX);
		store.setFilterSetExcitationFilterRef("Filter:1", INSTRUMENT_INDEX,
				FILTER_SET_INDEX, FILTER_INDEX);
		store.setFilterSetEmissionFilterRef("Filter:6", INSTRUMENT_INDEX,
				FILTER_SET_INDEX + 1, FILTER_INDEX);
		store.setFilterSetExcitationFilterRef("Filter:7", INSTRUMENT_INDEX,
				FILTER_SET_INDEX + 1, FILTER_INDEX);
		store.setLightPathEmissionFilterRef(
				"Filter:2", IMAGE_INDEX, CHANNEL_INDEX, CHANNEL_INDEX);
		store.setLightPathExcitationFilterRef(
				"Filter:3", IMAGE_INDEX, CHANNEL_INDEX, CHANNEL_INDEX);
		store.setLightPathEmissionFilterRef("Filter:4", IMAGE_INDEX,
        CHANNEL_INDEX + 1, CHANNEL_INDEX + 1);
		store.setLightPathExcitationFilterRef("Filter:5", IMAGE_INDEX,
        CHANNEL_INDEX + 1, CHANNEL_INDEX + 1);
	}

	/** Tests the color of the base channel.  */
	public void testBaseDataChannelOne()
	{
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(255, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(0, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNull(data.getLogicalChannel().getName());
	}
	
	
	/** Tests the color of the base channel two.  */
	public void testBaseDataChannelTwo()
	{
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX + 1);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNull(data.getLogicalChannel().getName());
	}
	
	/** Tests a graphic image. */
	public void testGraphicsDomain()
	{
		ChannelProcessor processor = new ChannelProcessor();
		store.setReader(new TestReader(true));
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(255, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(0, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals(ChannelProcessor.RED_TEXT, 
				data.getLogicalChannel().getName().getValue());
	}
	
	/** Tests an image with a logical channel with emission wavelength. */
	public void testLogicalChannelGreenEmissionWavelength()
	{
		store.setChannelEmissionWavelength(
        new PositiveInteger(525), IMAGE_INDEX, CHANNEL_INDEX);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(255, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(0, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("525", data.getLogicalChannel().getName().getValue());
	}
	
	/** Tests an image with a logical channel with emission wavelength. */
	public void testLogicalChannelBlueEmissionWavelength()
	{
		store.setChannelEmissionWavelength(
        new PositiveInteger(450), IMAGE_INDEX, CHANNEL_INDEX);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("450", data.getLogicalChannel().getName().getValue());
	}
	
	/** Tests an image with a logical channel with emission wavelength. */
	public void testLogicalChannelRedEmissionWavelength()
	{
		store.setChannelEmissionWavelength(
        new PositiveInteger(625), IMAGE_INDEX, CHANNEL_INDEX);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(255, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(0, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("625", data.getLogicalChannel().getName().getValue());
	}
	
	/** 
	 * Tests an image with a logical channel with filter set 
	 * with emission filter. 
	 */
	public void testFilterSetEmFilterBlueWavelength()
	{
		store.setTransmittanceRangeCutIn(425, INSTRUMENT_INDEX, 0);
		store.setTransmittanceRangeCutOut(430, INSTRUMENT_INDEX, 0);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("425", data.getLogicalChannel().getName().getValue());
	}
	
	/** 
	 * Tests an image with a logical channel with a laser.
	 */
	public void testLaserBlueWavelength()
	{
		store.setLaserWavelength(
        new PositiveInteger(435), INSTRUMENT_INDEX, LIGHTSOURCE_INDEX);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("435", data.getLogicalChannel().getName().getValue());
	}
	
	/** 
	 * Tests an image with a logical channel with excitation wavelength.
	 */
	public void testLogicalChannelGreenExcitationWavelength()
	{
		store.setChannelExcitationWavelength(
        new PositiveInteger(525), IMAGE_INDEX, CHANNEL_INDEX);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(255, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(0, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("525", data.getLogicalChannel().getName().getValue());
	}
	
	/** 
	 * Tests an image with a logical channel with excitation wavelength.
	 */
	public void testLogicalChannelBlueExcitationWavelength()
	{
		store.setChannelExcitationWavelength(
        new PositiveInteger(450), IMAGE_INDEX, CHANNEL_INDEX);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("450", data.getLogicalChannel().getName().getValue());
	}
	
	/** 
	 * Tests an image with a logical channel with excitation wavelength.
	 */
	public void testLogicalChannelRedExcitationWavelength()
	{
		store.setChannelExcitationWavelength(
        new PositiveInteger(625), IMAGE_INDEX, CHANNEL_INDEX);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(255, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(0, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("625", data.getLogicalChannel().getName().getValue());
	}
	
	/** 
	 * Tests an image with a logical channel with a filter set with an 
	 * excitation filter.
	 */
	public void testFilterSetExFilterBlueWavelength()
	{
		store.setTransmittanceRangeCutIn(425, INSTRUMENT_INDEX, 1);
		store.setTransmittanceRangeCutOut(430, INSTRUMENT_INDEX, 1);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("425", data.getLogicalChannel().getName().getValue());
	}
	
	/**
	 * Tests a logical channel with a light path with an emission filter.
	 */
	public void testLogicalChannelLightPathEmFilterBlueWavelength()
	{
		store.setTransmittanceRangeCutIn(430, INSTRUMENT_INDEX, 2);
		store.setTransmittanceRangeCutOut(435, INSTRUMENT_INDEX, 2);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("430", data.getLogicalChannel().getName().getValue());
	}
	
	/** 
	 * Tests a logical channel with a light path with an excitation filter.
	 */
	public void testLogicalChannelLightPathExFilterBlueWavelength()
	{
		store.setTransmittanceRangeCutIn(430, INSTRUMENT_INDEX, 3);
		store.setTransmittanceRangeCutOut(435, INSTRUMENT_INDEX, 3);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("430", data.getLogicalChannel().getName().getValue());
	}
	
	/**
	 * Tests a logical channel with a light path with an emission filter.
	 * and an emission filter from a filter set. 
	 * The value of the emission filter from the light path
	 * will determine the name and color.
	 */
	public void testLogicalChannelLightPathEmFilterBlueAndFilterSetEmFilterRedWavelength()
	{
		store.setTransmittanceRangeCutIn(430, INSTRUMENT_INDEX, 2);
		store.setTransmittanceRangeCutOut(435, INSTRUMENT_INDEX, 2);
		store.setTransmittanceRangeCutIn(625, INSTRUMENT_INDEX, 0);
		store.setTransmittanceRangeCutOut(640, INSTRUMENT_INDEX, 0);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("430", data.getLogicalChannel().getName().getValue());
	}
	
	/**
	 * Tests a logical channel with a light path with an excitation filter.
	 * and an excitation filter from a filter set. 
	 * The value of the excitation filter from the light path
	 * will determine the name and color.
	 */
	public void testLogicalChannelLightPathExFilterBlueAndFilterSetExFilterRedWavelength()
	{
		store.setTransmittanceRangeCutIn(430, INSTRUMENT_INDEX, 3);
		store.setTransmittanceRangeCutOut(435, INSTRUMENT_INDEX, 3);
		store.setTransmittanceRangeCutIn(625, INSTRUMENT_INDEX, 1);
		store.setTransmittanceRangeCutOut(640, INSTRUMENT_INDEX, 1);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("430", data.getLogicalChannel().getName().getValue());
	}
	
	/**
	 * Tests a logical channel with a light path with an excitation filter.
	 * and an emission filter set from a filter set. 
	 * The value of the emission filter from the filter set
	 * will determine the name and color.
	 */
	public void testLogicalChannelLightPathExFilterBlueAndFilterSetEmFilterRedWavelength()
	{
		store.setTransmittanceRangeCutIn(430, INSTRUMENT_INDEX, 3);
		store.setTransmittanceRangeCutOut(435, INSTRUMENT_INDEX, 3);
		store.setTransmittanceRangeCutIn(625, INSTRUMENT_INDEX, 0);
		store.setTransmittanceRangeCutOut(640, INSTRUMENT_INDEX, 0);
		ChannelProcessor processor = new ChannelProcessor();
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(255, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(0, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		assertNotNull(data.getLogicalChannel());
		assertNotNull(data.getLogicalChannel().getName());
		assertEquals("625", data.getLogicalChannel().getName().getValue());
	}
	
	/**
	 * Tests an image with 2 channels, one blue (light path emission filter)
	 * and another one with a transmitted light.
	 */
	public void testChannelsEmFilterLightPathBlueAndTransmittedLight()
	{
		ChannelProcessor processor = new ChannelProcessor();
		store.setReader(new TestReader(false));
		store.setTransmittanceRangeCutIn(430, INSTRUMENT_INDEX, 2);
		store.setTransmittanceRangeCutOut(435, INSTRUMENT_INDEX, 2);
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		//
		data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX+1);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(255, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(255, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
	}
	
	/**
	 * Tests an image with 2 channels, one red (light path emission filter) 
	 * and another one with a transmitted light.
	 */
	public void testChannelsEmFilterLightPathRedAndTransmittedLight()
	{
		ChannelProcessor processor = new ChannelProcessor();
		store.setReader(new TestReader(false));
		store.setTransmittanceRangeCutIn(600, INSTRUMENT_INDEX, 2);
		store.setTransmittanceRangeCutOut(620, INSTRUMENT_INDEX, 2);
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(255, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(0, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(0, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		//
		data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX+1);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(255, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(255, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
	}
	
	/**
	 * Tests an image with 2 channels, one green (light path emission filter) 
	 * and another one with a transmitted light.
	 */
	public void testChannelsEmFilterLightPathGreenAndTransmittedLight()
	{
		ChannelProcessor processor = new ChannelProcessor();
		store.setReader(new TestReader(false));
		store.setTransmittanceRangeCutIn(510, INSTRUMENT_INDEX, 2);
		store.setTransmittanceRangeCutOut(520, INSTRUMENT_INDEX, 2);
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(255, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(0, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		//
		data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX+1);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(255, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(255, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
	}
	
	/**
	 * Tests an image with 2 channels, one green (filter set emission filter) 
	 * and another one with a transmitted light.
	 */
	public void testChannelsEmFilterFilterSetGreenAndTransmittedLight()
	{
		ChannelProcessor processor = new ChannelProcessor();
		store.setReader(new TestReader(false));
		store.setTransmittanceRangeCutIn(510, INSTRUMENT_INDEX, 0);
		store.setTransmittanceRangeCutOut(520, INSTRUMENT_INDEX, 0);
		processor.process(store);
		ChannelData data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(0, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(255, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(0, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
		//
		data = ChannelData.fromObjectContainerStore(
				store, IMAGE_INDEX, CHANNEL_INDEX+1);
		assertNotNull(data.getChannel());
		assertNotNull(data.getChannel().getRed());
		assertEquals(255, data.getChannel().getRed().getValue());
		assertNotNull(data.getChannel().getGreen());
		assertEquals(255, data.getChannel().getGreen().getValue());
		assertNotNull(data.getChannel().getBlue());
		assertEquals(255, data.getChannel().getBlue().getValue());
		assertNotNull(data.getChannel().getAlpha());
		assertEquals(255, data.getChannel().getAlpha().getValue());
	}

}
