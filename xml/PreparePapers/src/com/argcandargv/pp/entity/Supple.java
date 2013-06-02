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
    "suppleFile",
    "suppleLinkDisp",
    "suppleText",
    "accessFlag",
    "collection"
})
@XmlRootElement(name = "supple")
public class Supple {

    @XmlElement(name = "supple_file", required = true)
    protected String suppleFile;
    @XmlElement(name = "supple_link_disp", required = true)
    protected String suppleLinkDisp;
    @XmlElement(name = "supple_text", required = true)
    protected String suppleText;
    @XmlElement(name = "access_flag", required = true)
    protected String accessFlag;
    @XmlElement(required = true)
    protected String collection;

    /**
     * Gets the value of the suppleFile property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSuppleFile() {
        return suppleFile;
    }

    /**
     * Sets the value of the suppleFile property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSuppleFile(String value) {
        this.suppleFile = value;
    }

    /**
     * Gets the value of the suppleLinkDisp property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSuppleLinkDisp() {
        return suppleLinkDisp;
    }

    /**
     * Sets the value of the suppleLinkDisp property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSuppleLinkDisp(String value) {
        this.suppleLinkDisp = value;
    }

    /**
     * Gets the value of the suppleText property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSuppleText() {
        return suppleText;
    }

    /**
     * Sets the value of the suppleText property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSuppleText(String value) {
        this.suppleText = value;
    }

    /**
     * Gets the value of the accessFlag property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getAccessFlag() {
        return accessFlag;
    }

    /**
     * Sets the value of the accessFlag property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setAccessFlag(String value) {
        this.accessFlag = value;
    }

    /**
     * Gets the value of the collection property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCollection() {
        return collection;
    }

    /**
     * Sets the value of the collection property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCollection(String value) {
        this.collection = value;
    }

}
