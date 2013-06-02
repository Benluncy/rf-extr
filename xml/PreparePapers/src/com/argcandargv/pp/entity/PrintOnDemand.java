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
    "displayCopyrightEmail",
    "displayCopyrightAddress",
    "distributedBy"
})
@XmlRootElement(name = "print_on_demand")
public class PrintOnDemand {

    @XmlElement(name = "display_copyright_email")
    protected String displayCopyrightEmail;
    @XmlElement(name = "display_copyright_address")
    protected String displayCopyrightAddress;
    @XmlElement(name = "distributed_by")
    protected String distributedBy;

    /**
     * Gets the value of the displayCopyrightEmail property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDisplayCopyrightEmail() {
        return displayCopyrightEmail;
    }

    /**
     * Sets the value of the displayCopyrightEmail property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDisplayCopyrightEmail(String value) {
        this.displayCopyrightEmail = value;
    }

    /**
     * Gets the value of the displayCopyrightAddress property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDisplayCopyrightAddress() {
        return displayCopyrightAddress;
    }

    /**
     * Sets the value of the displayCopyrightAddress property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDisplayCopyrightAddress(String value) {
        this.displayCopyrightAddress = value;
    }

    /**
     * Gets the value of the distributedBy property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDistributedBy() {
        return distributedBy;
    }

    /**
     * Sets the value of the distributedBy property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDistributedBy(String value) {
        this.distributedBy = value;
    }

}
