package com.argcandargv.pp.entity;


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
    "seriesId",
    "seriesTitle",
    "seriesVol"
})
@XmlRootElement(name = "series_name")
public class SeriesName {

    @XmlElement(name = "series_id")
    protected String seriesId;
    @XmlElement(name = "series_title", required = true)
    protected String seriesTitle;
    @XmlElement(name = "series_vol")
    protected String seriesVol;

    /**
     * Gets the value of the seriesId property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSeriesId() {
        return seriesId;
    }

    /**
     * Sets the value of the seriesId property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSeriesId(String value) {
        this.seriesId = value;
    }

    /**
     * Gets the value of the seriesTitle property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSeriesTitle() {
        return seriesTitle;
    }

    /**
     * Sets the value of the seriesTitle property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSeriesTitle(String value) {
        this.seriesTitle = value;
    }

    /**
     * Gets the value of the seriesVol property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSeriesVol() {
        return seriesVol;
    }

    /**
     * Sets the value of the seriesVol property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSeriesVol(String value) {
        this.seriesVol = value;
    }

}
