package com.argcandargv.pp.entity;


import java.util.ArrayList;
import java.util.List;
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
    "primaryCategory",
    "otherCategory"
})
@XmlRootElement(name = "categories")
public class Categories {

    @XmlElement(name = "primary_category", required = true)
    protected PrimaryCategory primaryCategory;
    @XmlElement(name = "other_category")
    protected List<OtherCategory> otherCategory;

    /**
     * Gets the value of the primaryCategory property.
     * 
     * @return
     *     possible object is
     *     {@link PrimaryCategory }
     *     
     */
    public PrimaryCategory getPrimaryCategory() {
        return primaryCategory;
    }

    /**
     * Sets the value of the primaryCategory property.
     * 
     * @param value
     *     allowed object is
     *     {@link PrimaryCategory }
     *     
     */
    public void setPrimaryCategory(PrimaryCategory value) {
        this.primaryCategory = value;
    }

    /**
     * Gets the value of the otherCategory property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the otherCategory property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getOtherCategory().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link OtherCategory }
     * 
     * 
     */
    public List<OtherCategory> getOtherCategory() {
        if (otherCategory == null) {
            otherCategory = new ArrayList<OtherCategory>();
        }
        return this.otherCategory;
    }

}
