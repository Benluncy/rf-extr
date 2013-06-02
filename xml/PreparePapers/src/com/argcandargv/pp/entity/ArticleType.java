package com.argcandargv.pp.entity;


import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;
import javax.xml.bind.annotation.adapters.CollapsedStringAdapter;
import javax.xml.bind.annotation.adapters.XmlJavaTypeAdapter;


/**
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "")
@XmlRootElement(name = "article_type")
public class ArticleType {

    @XmlAttribute(name = "art_type")
    @XmlJavaTypeAdapter(CollapsedStringAdapter.class)
    protected String artType;

    /**
     * Gets the value of the artType property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getArtType() {
        return artType;
    }

    /**
     * Sets the value of the artType property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setArtType(String value) {
        this.artType = value;
    }

}
