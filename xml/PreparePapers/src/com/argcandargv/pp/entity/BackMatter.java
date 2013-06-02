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
    "bmFile",
    "bmText"
})
@XmlRootElement(name = "back_matter")
public class BackMatter {

    @XmlElement(name = "bm_file", required = true)
    protected String bmFile;
    @XmlElement(name = "bm_text", required = true)
    protected String bmText;

    /**
     * Gets the value of the bmFile property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getBmFile() {
        return bmFile;
    }

    /**
     * Sets the value of the bmFile property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setBmFile(String value) {
        this.bmFile = value;
    }

    /**
     * Gets the value of the bmText property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getBmText() {
        return bmText;
    }

    /**
     * Sets the value of the bmText property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setBmText(String value) {
        this.bmText = value;
    }

}
