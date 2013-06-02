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
    "chEd"
})
@XmlRootElement(name = "chair_editor")
public class ChairEditor {

    @XmlElement(name = "ch_ed", required = true)
    protected List<ChEd> chEd;

    /**
     * Gets the value of the chEd property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the chEd property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getChEd().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ChEd }
     * 
     * 
     */
    public List<ChEd> getChEd() {
        if (chEd == null) {
            chEd = new ArrayList<ChEd>();
        }
        return this.chEd;
    }

}
