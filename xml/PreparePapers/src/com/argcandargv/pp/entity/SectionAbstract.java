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
    "par"
})
@XmlRootElement(name = "section_abstract")
public class SectionAbstract {

    @XmlElement(required = true)
    protected List<Par> par;

    /**
     * Gets the value of the par property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the par property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getPar().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link Par }
     * 
     * 
     */
    public List<Par> getPar() {
        if (par == null) {
            par = new ArrayList<Par>();
        }
        return this.par;
    }

}
