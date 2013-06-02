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
    "fmFile",
    "fmText"
})
@XmlRootElement(name = "front_matter")
public class FrontMatter {

    @XmlElement(name = "fm_file", required = true)
    protected String fmFile;
    @XmlElement(name = "fm_text", required = true)
    protected String fmText;

    /**
     * Gets the value of the fmFile property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getFmFile() {
        return fmFile;
    }

    /**
     * Sets the value of the fmFile property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setFmFile(String value) {
        this.fmFile = value;
    }

    /**
     * Gets the value of the fmText property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getFmText() {
        return fmText;
    }

    /**
     * Sets the value of the fmText property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setFmText(String value) {
        this.fmText = value;
    }

}
