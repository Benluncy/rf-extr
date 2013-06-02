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
@XmlRootElement(name = "qualifiers")
public class Qualifiers {

    @XmlAttribute(name = "density")
    @XmlJavaTypeAdapter(CollapsedStringAdapter.class)
    protected String density;
    @XmlAttribute(name = "review_process")
    @XmlJavaTypeAdapter(CollapsedStringAdapter.class)
    protected String reviewProcess;
    @XmlAttribute(name = "editorial_treatment")
    @XmlJavaTypeAdapter(CollapsedStringAdapter.class)
    protected String editorialTreatment;

    /**
     * Gets the value of the density property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDensity() {
        return density;
    }

    /**
     * Sets the value of the density property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDensity(String value) {
        this.density = value;
    }

    /**
     * Gets the value of the reviewProcess property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getReviewProcess() {
        return reviewProcess;
    }

    /**
     * Sets the value of the reviewProcess property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setReviewProcess(String value) {
        this.reviewProcess = value;
    }

    /**
     * Gets the value of the editorialTreatment property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getEditorialTreatment() {
        return editorialTreatment;
    }

    /**
     * Sets the value of the editorialTreatment property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setEditorialTreatment(String value) {
        this.editorialTreatment = value;
    }

}
