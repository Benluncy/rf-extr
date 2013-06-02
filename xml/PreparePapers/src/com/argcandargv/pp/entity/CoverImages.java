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
    "fullImage",
    "thumbnail"
})
@XmlRootElement(name = "cover_images")
public class CoverImages {

    @XmlElement(name = "full_image")
    protected String fullImage;
    protected String thumbnail;

    /**
     * Gets the value of the fullImage property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getFullImage() {
        return fullImage;
    }

    /**
     * Sets the value of the fullImage property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setFullImage(String value) {
        this.fullImage = value;
    }

    /**
     * Gets the value of the thumbnail property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getThumbnail() {
        return thumbnail;
    }

    /**
     * Sets the value of the thumbnail property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setThumbnail(String value) {
        this.thumbnail = value;
    }

}
