package com.argcandargv.pp.entity;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;
import javax.xml.bind.annotation.adapters.NormalizedStringAdapter;
import javax.xml.bind.annotation.adapters.XmlJavaTypeAdapter;


/**
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "", propOrder = {
    "comment",
    "conferenceRec",
    "seriesRec",
    "proceedingRec",
    "content"
})
@XmlRootElement(name = "proceeding")
public class Proceeding {

    @XmlAttribute(name = "ver", required = true)
    @XmlJavaTypeAdapter(NormalizedStringAdapter.class)
    protected String ver;
    @XmlAttribute(name = "ts", required = true)
    @XmlJavaTypeAdapter(NormalizedStringAdapter.class)
    protected String ts;
    protected List<Comment> comment;
    @XmlElement(name = "conference_rec", required = true)
    protected ConferenceRec conferenceRec;
    @XmlElement(name = "series_rec")
    protected SeriesRec seriesRec;
    @XmlElement(name = "proceeding_rec", required = true)
    protected ProceedingRec proceedingRec;
    protected List<Content> content;

    /**
     * Gets the value of the ver property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getVer() {
        return ver;
    }

    /**
     * Sets the value of the ver property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setVer(String value) {
        this.ver = value;
    }

    /**
     * Gets the value of the ts property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTs() {
        return ts;
    }

    /**
     * Sets the value of the ts property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTs(String value) {
        this.ts = value;
    }

    /**
     * Gets the value of the comment property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the comment property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getComment().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link Comment }
     * 
     * 
     */
    public List<Comment> getComment() {
        if (comment == null) {
            comment = new ArrayList<Comment>();
        }
        return this.comment;
    }

    /**
     * Gets the value of the conferenceRec property.
     * 
     * @return
     *     possible object is
     *     {@link ConferenceRec }
     *     
     */
    public ConferenceRec getConferenceRec() {
        return conferenceRec;
    }

    /**
     * Sets the value of the conferenceRec property.
     * 
     * @param value
     *     allowed object is
     *     {@link ConferenceRec }
     *     
     */
    public void setConferenceRec(ConferenceRec value) {
        this.conferenceRec = value;
    }

    /**
     * Gets the value of the seriesRec property.
     * 
     * @return
     *     possible object is
     *     {@link SeriesRec }
     *     
     */
    public SeriesRec getSeriesRec() {
        return seriesRec;
    }

    /**
     * Sets the value of the seriesRec property.
     * 
     * @param value
     *     allowed object is
     *     {@link SeriesRec }
     *     
     */
    public void setSeriesRec(SeriesRec value) {
        this.seriesRec = value;
    }

    /**
     * Gets the value of the proceedingRec property.
     * 
     * @return
     *     possible object is
     *     {@link ProceedingRec }
     *     
     */
    public ProceedingRec getProceedingRec() {
        return proceedingRec;
    }

    /**
     * Sets the value of the proceedingRec property.
     * 
     * @param value
     *     allowed object is
     *     {@link ProceedingRec }
     *     
     */
    public void setProceedingRec(ProceedingRec value) {
        this.proceedingRec = value;
    }

    /**
     * Gets the value of the content property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the content property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getContent().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link Content }
     * 
     * 
     */
    public List<Content> getContent() {
        if (content == null) {
            content = new ArrayList<Content>();
        }
        return this.content;
    }

}
