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
    "publisherId",
    "publisherCode",
    "publisherName",
    "publisherAddress",
    "publisherCity",
    "publisherState",
    "publisherCountry",
    "publisherZipCode",
    "publisherContact",
    "publisherPhone",
    "publisherIsbnPrefix",
    "publisherUrl"
})
@XmlRootElement(name = "publisher")
public class Publisher {

    @XmlElement(name = "publisher_id")
    protected String publisherId;
    @XmlElement(name = "publisher_code", required = true)
    protected String publisherCode;
    @XmlElement(name = "publisher_name")
    protected String publisherName;
    @XmlElement(name = "publisher_address")
    protected String publisherAddress;
    @XmlElement(name = "publisher_city")
    protected String publisherCity;
    @XmlElement(name = "publisher_state")
    protected String publisherState;
    @XmlElement(name = "publisher_country")
    protected String publisherCountry;
    @XmlElement(name = "publisher_zip_code")
    protected String publisherZipCode;
    @XmlElement(name = "publisher_contact")
    protected String publisherContact;
    @XmlElement(name = "publisher_phone")
    protected String publisherPhone;
    @XmlElement(name = "publisher_isbn_prefix")
    protected String publisherIsbnPrefix;
    @XmlElement(name = "publisher_url")
    protected String publisherUrl;

    /**
     * Gets the value of the publisherId property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherId() {
        return publisherId;
    }

    /**
     * Sets the value of the publisherId property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherId(String value) {
        this.publisherId = value;
    }

    /**
     * Gets the value of the publisherCode property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherCode() {
        return publisherCode;
    }

    /**
     * Sets the value of the publisherCode property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherCode(String value) {
        this.publisherCode = value;
    }

    /**
     * Gets the value of the publisherName property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherName() {
        return publisherName;
    }

    /**
     * Sets the value of the publisherName property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherName(String value) {
        this.publisherName = value;
    }

    /**
     * Gets the value of the publisherAddress property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherAddress() {
        return publisherAddress;
    }

    /**
     * Sets the value of the publisherAddress property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherAddress(String value) {
        this.publisherAddress = value;
    }

    /**
     * Gets the value of the publisherCity property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherCity() {
        return publisherCity;
    }

    /**
     * Sets the value of the publisherCity property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherCity(String value) {
        this.publisherCity = value;
    }

    /**
     * Gets the value of the publisherState property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherState() {
        return publisherState;
    }

    /**
     * Sets the value of the publisherState property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherState(String value) {
        this.publisherState = value;
    }

    /**
     * Gets the value of the publisherCountry property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherCountry() {
        return publisherCountry;
    }

    /**
     * Sets the value of the publisherCountry property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherCountry(String value) {
        this.publisherCountry = value;
    }

    /**
     * Gets the value of the publisherZipCode property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherZipCode() {
        return publisherZipCode;
    }

    /**
     * Sets the value of the publisherZipCode property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherZipCode(String value) {
        this.publisherZipCode = value;
    }

    /**
     * Gets the value of the publisherContact property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherContact() {
        return publisherContact;
    }

    /**
     * Sets the value of the publisherContact property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherContact(String value) {
        this.publisherContact = value;
    }

    /**
     * Gets the value of the publisherPhone property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherPhone() {
        return publisherPhone;
    }

    /**
     * Sets the value of the publisherPhone property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherPhone(String value) {
        this.publisherPhone = value;
    }

    /**
     * Gets the value of the publisherIsbnPrefix property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherIsbnPrefix() {
        return publisherIsbnPrefix;
    }

    /**
     * Sets the value of the publisherIsbnPrefix property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherIsbnPrefix(String value) {
        this.publisherIsbnPrefix = value;
    }

    /**
     * Gets the value of the publisherUrl property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublisherUrl() {
        return publisherUrl;
    }

    /**
     * Sets the value of the publisherUrl property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublisherUrl(String value) {
        this.publisherUrl = value;
    }

}
