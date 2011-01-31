/*
 * org.openmicroscopy.shoola.agents.fsimporter.view.ImporterUIElement 
 *
 *------------------------------------------------------------------------------
 *  Copyright (C) 2006-2010 University of Dundee. All rights reserved.
 *
 *
 * 	This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *------------------------------------------------------------------------------
 */
package org.openmicroscopy.shoola.agents.fsimporter.view;


//Java imports
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Map.Entry;
import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.border.LineBorder;

//Third-party libraries
import info.clearthought.layout.TableLayout;

//Application-internal dependencies
import org.openmicroscopy.shoola.agents.events.importer.BrowseContainer;
import org.openmicroscopy.shoola.agents.events.importer.ImportStatusEvent;
import org.openmicroscopy.shoola.agents.fsimporter.ImporterAgent;
import org.openmicroscopy.shoola.agents.fsimporter.util.FileImportComponent;
import org.openmicroscopy.shoola.agents.util.browser.TreeImageDisplay;
import org.openmicroscopy.shoola.env.data.model.ImportableFile;
import org.openmicroscopy.shoola.env.data.model.ImportableObject;
import org.openmicroscopy.shoola.env.event.EventBus;
import org.openmicroscopy.shoola.util.ui.ClosableTabbedPaneComponent;
import org.openmicroscopy.shoola.util.ui.UIUtilities;
import pojos.DataObject;
import pojos.DatasetData;
import pojos.ProjectData;
import pojos.ScreenData;
import pojos.TagAnnotationData;

/** 
 * Component displaying an import.
 *
 * @author Jean-Marie Burel &nbsp;&nbsp;&nbsp;&nbsp;
 * <a href="mailto:j.burel@dundee.ac.uk">j.burel@dundee.ac.uk</a>
 * @author Donald MacDonald &nbsp;&nbsp;&nbsp;&nbsp;
 * <a href="mailto:donald@lifesci.dundee.ac.uk">donald@lifesci.dundee.ac.uk</a>
 * @version 3.0
 * <small>
 * (<b>Internal version:</b> $Revision: $Date: $)
 * </small>
 * @since 3.0-Beta4
 */
class ImporterUIElement 
	extends ClosableTabbedPaneComponent
{

	/** The object hosting information about files to import. */
	private ImportableObject object;
	
	/** The components to lay out. */
	private LinkedHashMap<String, FileImportComponent>	components;

	/** Component hosting the entries. */
	private JPanel	entries;
	
	/** The columns for the layout of the {@link #entries}. */
	private static final double[] COLUMNS = {TableLayout.FILL};
	
	/** The number of files imported. */
	private int countImported;
	
	/** The total number of tiles to import. */
	private int totalToImport;
	
	/** The time when the import started. */
	private long startImport;
	
	/** The component displaying the duration of the import. */
	private JLabel timeLabel;
	
	/** Flag indicating if the import is finished. */
	private boolean done;
	
	/** The identifier of the component. */
	private int id;
	
	/** The collection of folders' name used as dataset. */
	private Map<JLabel, FileImportComponent> foldersName;
	
	/** Flag indicating that the images will be added to the default dataset. */
	private boolean orphanedFiles;
	
	/** Reference to the controller. */
	private ImporterControl controller;
	
	/** The components displaying the components. */
	private Map<JLabel, Object> containerComponents;
	
	/** The components displaying the components. */
	private Map<JLabel, Object> topContainerComponents;
	
	/** Flag indicating to refresh the topContainer. */
	private boolean topContainerToRefresh;
	
	/** The listener associated to the folder. */
	private MouseAdapter folderListener;
	
	/** The listener associated to the container. */
	private MouseAdapter containerListener;
	
	/**
	 * Browses the specified object.
	 * 
	 * @param data The object to handle.
	 * @param node The node hosting the object to browse or <code>null</code>.
	 */ 
	private void browse(Object data, Object node)
	{
		EventBus bus = ImporterAgent.getRegistry().getEventBus();
		if (data instanceof TreeImageDisplay || data instanceof DataObject) {
			bus.post(new BrowseContainer(data, node));
		} else if (data instanceof FileImportComponent) {
			FileImportComponent fc = (FileImportComponent) data;
			bus.post(new BrowseContainer(fc.getContainerFromFolder(), node));
		}
	}
	
	/**
	 * Returns the label hosting the passed object.
	 * 
	 * @param data The object to handle.
	 * @return See above.
	 */
	private JLabel createNameLabel(Object data)
	{
		JLabel label = new JLabel();
		boolean browse = false;
		String name = "";
		if (data instanceof DatasetData) {
			browse = true;
			name = ((DatasetData) data).getName();
		} else if (data instanceof ScreenData) {
			browse = true;
			name = ((ScreenData) data).getName();
		} else if (data instanceof ProjectData) {
			browse = true;
			name = ((ProjectData) data).getName();
		}
		label.setEnabled(browse);
		if (browse) {
			label.setBackground(UIUtilities.BACKGROUND_COLOR);
			label.setToolTipText("Double click to browse when import " +
					"completed.");
		}
		label.setText(name);
		return label;
	}
	
	/** Initializes the components. */
	private void initialize()
	{
		containerComponents = new LinkedHashMap<JLabel, Object>();
		topContainerComponents = new LinkedHashMap<JLabel, Object>();
		foldersName = new LinkedHashMap<JLabel, FileImportComponent>();
		folderListener = new MouseAdapter() {
			
			/**
			 * Browses the object the image.
			 * @see MouseListener#mousePressed(MouseEvent)
			 */
			public void mousePressed(MouseEvent e)
			{
				Object src = e.getSource();
				if (e.getClickCount() == 2 && src instanceof JLabel) {
					browse(foldersName.get((JLabel) src), null);
				}
			}
		};
		containerListener = new MouseAdapter() {
			
			/**
			 * Browses the object the image.
			 * @see MouseListener#mousePressed(MouseEvent)
			 */
			public void mousePressed(MouseEvent e)
			{
				Object src = e.getSource();
				if (e.getClickCount() == 2 && src instanceof JLabel) {
					browse(containerComponents.get((JLabel) src), null);
				}
			}
		};
		countImported = 0;
		setClosable(true);
		addPropertyChangeListener(controller);
		entries = new JPanel();
		entries.setBackground(UIUtilities.BACKGROUND);
		components = new LinkedHashMap<String, FileImportComponent>();
		List<ImportableFile> files = object.getFiles();
		FileImportComponent c;
		File f;
		Iterator<ImportableFile> i = files.iterator();
		orphanedFiles = false;
		ImportableFile importable;
		int type = -1;
		List<Object> containers = object.getRefNodes();
		if (containers != null && containers.size() > 0) {
			Iterator<Object> j = containers.iterator();
			TreeImageDisplay node;
			Object h;
			while (j.hasNext()) {
				node = (TreeImageDisplay) j.next();
				h = node.getUserObject();
				if (h instanceof DatasetData) {
					type = FileImportComponent.DATASET_TYPE;
				} else if (h instanceof ScreenData) {
					type = FileImportComponent.SCREEN_TYPE;
				} else if (h instanceof ProjectData) {
					type = FileImportComponent.PROJECT_TYPE;
				}
				break;
			}
		} else {
			type = FileImportComponent.NO_CONTAINER;
		}
		JLabel l;
		int count = 0;
		
		
		while (i.hasNext()) {
			importable = i.next();
			f = (File) importable.getFile();
			c = new FileImportComponent(f, importable.isFolderAsContainer());
			c.setType(type);
			c.addPropertyChangeListener(controller);
			c.addPropertyChangeListener(new PropertyChangeListener() {
				
				public void propertyChange(PropertyChangeEvent evt) {
					String name = evt.getPropertyName();
					if (FileImportComponent.BROWSE_PROPERTY.equals(name)) {
						List<Object> refNodes = object.getRefNodes();
						Object node = null;
						if (refNodes.size() > 0)
							node = refNodes.get(0);
						browse(evt.getNewValue(), node);
					}
				}
			});
			if (f.isDirectory()) {
				if (importable.isFolderAsContainer()) {
					l = new JLabel(f.getName());
					foldersName.put(l, c);
				}
				//else orphanedFiles = true;
			} else {
				count++;
				//else orphanedFiles = true;
			}
			importable.setStatus(c.getStatus());
			components.put(f.getAbsolutePath(), c);
		}
		DatasetData dataset = object.getDefaultDataset();
		if (dataset != null && count > 0) 
			containerComponents.put(createNameLabel(dataset), dataset);
		totalToImport = files.size();
	}
	
	/** 
	 * Builds and lays out the header.
	 * 
	 * @return See above.
	 */
	private JPanel buildHeader()
	{
		JPanel header = new JPanel();
		header.setBackground(UIUtilities.BACKGROUND_COLOR);
		header.setBorder(BorderFactory.createEmptyBorder(0, 5, 0, 5));
		header.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.anchor = GridBagConstraints.WEST;
		c.insets = new Insets(0, 2, 2, 0);
		c.gridy = 0;
		c.gridx = 0;
		JLabel label = UIUtilities.setTextFont("Import Time:", Font.BOLD);
		JLabel value;
		startImport = System.currentTimeMillis();
		timeLabel = UIUtilities.createComponent(null);
		timeLabel.setText(UIUtilities.formatShortDateTime(null));
    	header.add(label, c);
    	c.gridx = c.gridx+2;
    	header.add(timeLabel, c);
    	c.gridy++; 	
    	c.gridx = 0;
    	List<Object> containers = object.getRefNodes();
		String text = "Imported in Dataset: ";
		String name = "";
		int n;
		String nameProject = "";
		if (containers != null && containers.size() > 0) {
			Iterator<Object> i = containers.iterator();
			int index = 0;
			n = containers.size()-1;
			TreeImageDisplay node;
			Object h;
			while (i.hasNext()) {
				node = (TreeImageDisplay) i.next();
				h = node.getUserObject();
				if (h instanceof DatasetData) {
					containerComponents.put(createNameLabel(h), node);
					//if (index == 0) text += "Dataset: ";
					name += ((DatasetData) h).getName();
				} else if (h instanceof ScreenData) {
					containerComponents.put(createNameLabel(h), node);
					if (index == 0) text = "Imported in Screen: ";
					name += ((ScreenData) h).getName();
				} else if (h instanceof ProjectData) {
					//if (index == 0) text += "Project: ";
					//name += ((ProjectData) h).getName();
					topContainerComponents.put(createNameLabel(h), node);
					nameProject += ((ProjectData) h).getName();
				}
				if (index < n) name += ", ";
				index++;
			}
		} else {
			/*
			if (DatasetData.class.equals(object.getType())) {
				set = true;
				//text += "Dataset: ";
				n = foldersName.size();
				if (n > 0) {
					n = n-1;
					for (int j = 0; j < foldersName.size(); j++) {
						name += foldersName.get(j);
						if (j < n) name += ", ";
					}
				}
				
				if (orphanedFiles) {
					if (foldersName.size() > 0) name += ", ";
					name += UIUtilities.formatDate(null, 
							UIUtilities.D_M_Y_FORMAT);
				}
			}
			*/
		}
		/*
		if (DatasetData.class.equals(object.getType()) && !set) {
			n = foldersName.size();
			if (n > 0) {
				n = n-1;
				for (int j = 0; j < foldersName.size(); j++) {
					name += foldersName.get(j);
					if (j < n) name += ", ";
				}
			}
		}
		*/
		//Project List
		if (topContainerComponents.size() > 0) {
			label = UIUtilities.setTextFont("Imported in Project", Font.BOLD);
			//value = UIUtilities.createComponent(null);
	    	//value.setText(name);
			header.add(label, c);
	    	c.gridx = c.gridx+2;
	    	JPanel p = new JPanel();
    		p.setBackground(UIUtilities.BACKGROUND_COLOR);
    		p.setLayout(new FlowLayout(FlowLayout.LEFT));
    		Iterator<JLabel> l = topContainerComponents.keySet().iterator();
    		while (l.hasNext())
				p.add(l.next());
    		header.add(p, c);
	    	c.gridy++; 	
	    	c.gridx = 0;
		}
		if (text != null) {
			label = UIUtilities.setTextFont(text, Font.BOLD);
			//value = UIUtilities.createComponent(null);
	    	//value.setText(name);
			header.add(label, c);
	    	c.gridx = c.gridx+2;
	    	JPanel p = new JPanel();
    		p.setBackground(UIUtilities.BACKGROUND_COLOR);
    		p.setLayout(new FlowLayout(FlowLayout.LEFT));
    		Iterator<JLabel> l;
	    	if (containerComponents.size() > 0) {
	    		l = containerComponents.keySet().iterator();
	    		while (l.hasNext())
					p.add(l.next());
	    	} 
	    	if (foldersName.size() > 0) {
	    		l = foldersName.keySet().iterator();
	    		while (l.hasNext())
					p.add(l.next());
    		}
	    	
	    	header.add(p, c);
	    	c.gridy++; 	
	    	c.gridx = 0;
		}
		Collection<TagAnnotationData> tags = object.getTags();
		if (tags != null && tags.size() > 0) {
			label = UIUtilities.setTextFont("Images Tagged with: ", Font.BOLD);
			value = UIUtilities.createComponent(null);
			text = "";
			Iterator<TagAnnotationData> i = tags.iterator();
			int index = 0;
			n = tags.size()-1;
			while (i.hasNext()) {
				text += (i.next()).getTagValue();
				if (index < n) text +=", ";
				index++;
			}
			value.setText(text);
			header.add(label, c);
	    	c.gridx = c.gridx+2;
	    	header.add(value, c);
	    	c.gridy++; 	
	    	c.gridx = 0;
		}
		topContainerToRefresh = topContainerToRefresh();
		JPanel content = UIUtilities.buildComponentPanel(header);
		content.setBackground(UIUtilities.BACKGROUND_COLOR);
		return content;
	}
	
	/** Builds and lays out the UI. */
	private void buildGUI()
	{
		layoutEntries();
		JScrollPane pane = new JScrollPane(entries);
		pane.setOpaque(false);
		pane.setBorder(new LineBorder(Color.LIGHT_GRAY));
		setLayout(new BorderLayout(0, 0));
		add(buildHeader(), BorderLayout.NORTH);
		add(pane, BorderLayout.CENTER);
	}
	
	/** Lays out the entries. */
	private void layoutEntries()
	{
		entries.removeAll();
		TableLayout layout = new TableLayout();
		layout.setColumn(COLUMNS);
		entries.setLayout(layout);
		int index = 0;
		Entry entry;
		
		FileImportComponent c;
		Iterator i = components.entrySet().iterator();
		while (i.hasNext()) {
			entry = (Entry) i.next();
			c = (FileImportComponent) entry.getValue();
			addRow(layout, index, c);
			index++;
		}
		entries.revalidate();
		repaint();
	}
	
	/**
	 * Adds a new row.
	 * 
	 * @param layout The layout.
	 * @param index	 The index of the row.
	 * @param c		 The component to add.
	 */
	private void addRow(TableLayout layout, int index, FileImportComponent c)
	{
		layout.insertRow(index, TableLayout.PREFERRED);
		if (index%2 == 0)
			c.setBackground(UIUtilities.BACKGROUND_COLOUR_EVEN);
		else 
			c.setBackground(UIUtilities.BACKGROUND_COLOUR_ODD);
		//entries.add(c.getNameLabel(), "0, "+index);
		entries.add(c, "0, "+index+"");
	}
	
	/**
	 * Returns <code>true</code> if some files were imported, 
	 * <code>false</code> otherwise.
	 * 
	 * @return See above.
	 */
	private boolean toRefresh()
	{
		Entry entry;
		Iterator i = components.entrySet().iterator();
		FileImportComponent fc;
		while (i.hasNext()) {
			entry = (Entry) i.next();
			fc = (FileImportComponent) entry.getValue();
			if (fc.toRefresh()) 
				return true;
		}
		return false;
	}
	
	/**
	 * Creates a new instance.
	 * 
	 * @param controller Reference to the control. Mustn't be <code>null</code>.
	 * @param id The identifier of the component.
	 * @param index The index of the component.
	 * @param name The name of the component.
	 * @param object the object to handle. Mustn't be <code>null</code>.
	 */
	ImporterUIElement(ImporterControl controller, int id, int index, 
			String name, ImportableObject object)
	{
		super(index, name);
		if (object == null) 
			throw new IllegalArgumentException("No object specified.");
		if (controller == null)
			throw new IllegalArgumentException("No controller."); 
		this.controller = controller;
		this.id = id;
		this.object = object;
		initialize();
		buildGUI();
	}

	/**
	 * Returns the identifier of the component.
	 * 
	 * @return See above.
	 */
	int getID() { return id; }
	
	/**
	 * Returns <code>true</code> if the top container has to be refreshed,
	 * <code>false</code> otherwise.
	 * 
	 * @return See above.
	 */
	private boolean topContainerToRefresh()
	{
		List<DataObject> l = getData().getContainers();
		if (l == null || l.size() == 0) return false;
		DataObject object = l.get(0);
		if (!(object instanceof ProjectData)) return false;
		DatasetData d = getData().getDefaultDataset();
		if (d != null && d.getId() <= 0) return true;
		Iterator<FileImportComponent> i = components.values().iterator();
		FileImportComponent fc;
		while (i.hasNext()) {
			fc = i.next();
			if (fc.isFolderAsContainer()) 
				return true;
		}
		return false;
	}
	
	/**
	 * Sets the result of the import for the specified file.
	 * 
	 * @param f The file to import.
	 * @param result The result.
	 */
	void setImportedFile(File f, Object result)
	{
		FileImportComponent c = components.get(f.getAbsolutePath());
		if (c != null) {
			c.setStatus(false, result);
			countImported++;
			done = countImported == totalToImport;
			setClosable(done);
			if (done) {
				Iterator<JLabel> i = containerComponents.keySet().iterator();
				JLabel label;
				boolean toRefresh = toRefresh();
				if (toRefresh) {
					while (i.hasNext()) {
						label = i.next();
						if (label.isEnabled()) {
							label.setForeground(UIUtilities.HYPERLINK_COLOR);
							label.addMouseListener(containerListener);
						}
					}
				}
				if (topContainerToRefresh) {
					i = topContainerComponents.keySet().iterator();
					while (i.hasNext()) {
						label = i.next();
						if (label.isEnabled())
							label.setForeground(UIUtilities.HYPERLINK_COLOR);
					}
				}
				if (foldersName.size() > 0) {
					Entry entry;
					Iterator k = foldersName.entrySet().iterator();
					FileImportComponent fc;
					while (k.hasNext()) {
						entry = (Entry) k.next();
						label = (JLabel) entry.getKey();
						fc = (FileImportComponent) entry.getValue();
						if (fc.toRefresh()) {
							label.setForeground(UIUtilities.HYPERLINK_COLOR);
							label.addMouseListener(folderListener);
						}
					}
				}
				long duration = System.currentTimeMillis()-startImport;
				String text = timeLabel.getText();
				String time = UIUtilities.calculateHMS((int) (duration/1000));
				timeLabel.setText(text+" Duration: "+time);
				EventBus bus = ImporterAgent.getRegistry().getEventBus();
				ImportStatusEvent event;
				if (toRefresh) {
					List<DataObject> l = getData().getContainers();
					if (!topContainerToRefresh) {
						DatasetData d = getData().getDefaultDataset();
						if (d != null && d.getId() > 0) {
							l = new ArrayList<DataObject>();
							l.add(d);
						}
					}
					event = new ImportStatusEvent(false, l);
				} else {
					event = new ImportStatusEvent(false, null);
				}
				event.setToRefresh(hasToRefreshTree());
				bus.post(event);
			}
		}
	}
	
	/**
	 * Returns <code>true</code> if the import is finished, <code>false</code>
	 * otherwise.
	 * 
	 * @return See above.
	 */
	boolean isDone() { return done; }
	
	/** Indicates that the import has started. */
	void startImport()
	{ 
		setClosable(false);
		repaint();
	}
	
	/**
	 * Returns the collection of files that could not be imported.
	 * 
	 * @return See above.
	 */
	List<FileImportComponent> getMarkedFiles()
	{
		List<FileImportComponent> list = new ArrayList<FileImportComponent>();
		Entry entry;
		Iterator i = components.entrySet().iterator();
		FileImportComponent fc;
		File f;
		List<FileImportComponent> l;
		while (i.hasNext()) {
			entry = (Entry) i.next();
			fc = (FileImportComponent) entry.getValue();
			l = fc.getImportErrors();
			if (l != null && l.size() > 0)
				list.addAll(l);
		}
		return list;
	}
	
	/**
	 * Returns the object to import.
	 * 
	 * @return See above.
	 */
	ImportableObject getData() { return object; }
	
	/**
	 * Returns <code>true</code> if errors to send, <code>false</code>
	 * otherwise.
	 * 
	 * @return See above.
	 */
	boolean hasFailuresToSend()
	{
		Entry entry;
		Iterator i = components.entrySet().iterator();
		FileImportComponent fc;
		while (i.hasNext()) {
			entry = (Entry) i.next();
			fc = (FileImportComponent) entry.getValue();
			if (fc.hasFailuresToSend()) 
				return true;
		}
		return false;
	}

	/** Indicates that the import has been cancel. */
	void cancelLoading()
	{
		if (components == null || components.size() == 0) return;
		Iterator<FileImportComponent> i = components.values().iterator();
		while (i.hasNext()) {
			i.next().cancelLoading();
		}
	}

	/**
	 * Returns <code>true</code> if the view should be refreshed,
	 * <code>false</code> otherwise.
	 * 
	 * @return See above.
	 */
	boolean hasToRefreshTree()
	{
		Entry entry;
		Iterator i = components.entrySet().iterator();
		FileImportComponent fc;
		while (i.hasNext()) {
			entry = (Entry) i.next();
			fc = (FileImportComponent) entry.getValue();
			if (fc.hasToRefreshTree()) 
				return true;
		}
		return false;
	}
	
}
