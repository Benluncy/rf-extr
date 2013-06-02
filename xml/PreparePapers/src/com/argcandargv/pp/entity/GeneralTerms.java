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
    "gt"
})
@XmlRootElement(name = "general_terms")
public class GeneralTerms {

    @XmlElement(required = true)
    protected List<Gt> gt;

    /**
     * Gets the value of the gt property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the gt property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getGt().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link Gt }
     * 
     * 
     */
    public List<Gt> getGt() {
        if (gt == null) {
            gt = new ArrayList<Gt>();
        }
        return this.gt;
    }

}
