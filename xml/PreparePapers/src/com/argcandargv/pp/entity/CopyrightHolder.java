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
    "copyrightHolderName",
    "copyrightHolderYear",
    "cmsConfSortNo"
})
@XmlRootElement(name = "copyright_holder")
public class CopyrightHolder {

    @XmlElement(name = "copyright_holder_name", required = true)
    protected String copyrightHolderName;
    @XmlElement(name = "copyright_holder_year")
    protected String copyrightHolderYear;
    @XmlElement(name = "cms_conf_sort_no")
    protected String cmsConfSortNo;

    /**
     * Gets the value of the copyrightHolderName property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCopyrightHolderName() {
        return copyrightHolderName;
    }

    /**
     * Sets the value of the copyrightHolderName property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCopyrightHolderName(String value) {
        this.copyrightHolderName = value;
    }

    /**
     * Gets the value of the copyrightHolderYear property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCopyrightHolderYear() {
        return copyrightHolderYear;
    }

    /**
     * Sets the value of the copyrightHolderYear property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCopyrightHolderYear(String value) {
        this.copyrightHolderYear = value;
    }

    /**
     * Gets the value of the cmsConfSortNo property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCmsConfSortNo() {
        return cmsConfSortNo;
    }

    /**
     * Sets the value of the cmsConfSortNo property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCmsConfSortNo(String value) {
        this.cmsConfSortNo = value;
    }

}
