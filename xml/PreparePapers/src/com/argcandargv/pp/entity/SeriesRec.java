package com.argcandargv.pp.entity;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;


/**
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "", propOrder = {
    "seriesName"
})
@XmlRootElement(name = "series_rec")
public class SeriesRec {

    @XmlElement(name = "series_name", required = true)
    protected List<SeriesName> seriesName;

    /**
     * Gets the value of the seriesName property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the seriesName property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getSeriesName().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link SeriesName }
     * 
     * 
     */
    public List<SeriesName> getSeriesName() {
        if (seriesName == null) {
            seriesName = new ArrayList<SeriesName>();
        }
        return this.seriesName;
    }

}
