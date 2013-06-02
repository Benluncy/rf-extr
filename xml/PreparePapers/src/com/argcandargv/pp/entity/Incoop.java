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
    "incoopId",
    "incoopTitle",
    "incoopAbbr"
})
@XmlRootElement(name = "incoop")
public class Incoop {

    @XmlElement(name = "incoop_id")
    protected String incoopId;
    @XmlElement(name = "incoop_title", required = true)
    protected String incoopTitle;
    @XmlElement(name = "incoop_abbr")
    protected String incoopAbbr;

    /**
     * Gets the value of the incoopId property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIncoopId() {
        return incoopId;
    }

    /**
     * Sets the value of the incoopId property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIncoopId(String value) {
        this.incoopId = value;
    }

    /**
     * Gets the value of the incoopTitle property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIncoopTitle() {
        return incoopTitle;
    }

    /**
     * Sets the value of the incoopTitle property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIncoopTitle(String value) {
        this.incoopTitle = value;
    }

    /**
     * Gets the value of the incoopAbbr property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIncoopAbbr() {
        return incoopAbbr;
    }

    /**
     * Sets the value of the incoopAbbr property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIncoopAbbr(String value) {
        this.incoopAbbr = value;
    }

}
