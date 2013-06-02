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
    "sectionId",
    "sortKey",
    "sectionSeqNo",
    "sectionType",
    "sectionTitle",
    "sectionPageFrom",
    "sectionAbstract",
    "chairEditor",
    "fulltext",
    "articleRec"
})
@XmlRootElement(name = "section")
public class Section {

    @XmlElement(name = "section_id")
    protected String sectionId;
    @XmlElement(name = "sort_key", required = true)
    protected String sortKey;
    @XmlElement(name = "section_seq_no", required = true)
    protected String sectionSeqNo;
    @XmlElement(name = "section_type", required = true)
    protected String sectionType;
    @XmlElement(name = "section_title", required = true)
    protected String sectionTitle;
    @XmlElement(name = "section_page_from", required = true)
    protected String sectionPageFrom;
    @XmlElement(name = "section_abstract")
    protected SectionAbstract sectionAbstract;
    @XmlElement(name = "chair_editor")
    protected ChairEditor chairEditor;
    protected Fulltext fulltext;
    @XmlElement(name = "article_rec")
    protected List<ArticleRec> articleRec;

    /**
     * Gets the value of the sectionId property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSectionId() {
        return sectionId;
    }

    /**
     * Sets the value of the sectionId property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSectionId(String value) {
        this.sectionId = value;
    }

    /**
     * Gets the value of the sortKey property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSortKey() {
        return sortKey;
    }

    /**
     * Sets the value of the sortKey property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSortKey(String value) {
        this.sortKey = value;
    }

    /**
     * Gets the value of the sectionSeqNo property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSectionSeqNo() {
        return sectionSeqNo;
    }

    /**
     * Sets the value of the sectionSeqNo property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSectionSeqNo(String value) {
        this.sectionSeqNo = value;
    }

    /**
     * Gets the value of the sectionType property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSectionType() {
        return sectionType;
    }

    /**
     * Sets the value of the sectionType property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSectionType(String value) {
        this.sectionType = value;
    }

    /**
     * Gets the value of the sectionTitle property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSectionTitle() {
        return sectionTitle;
    }

    /**
     * Sets the value of the sectionTitle property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSectionTitle(String value) {
        this.sectionTitle = value;
    }

    /**
     * Gets the value of the sectionPageFrom property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSectionPageFrom() {
        return sectionPageFrom;
    }

    /**
     * Sets the value of the sectionPageFrom property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSectionPageFrom(String value) {
        this.sectionPageFrom = value;
    }

    /**
     * Gets the value of the sectionAbstract property.
     * 
     * @return
     *     possible object is
     *     {@link SectionAbstract }
     *     
     */
    public SectionAbstract getSectionAbstract() {
        return sectionAbstract;
    }

    /**
     * Sets the value of the sectionAbstract property.
     * 
     * @param value
     *     allowed object is
     *     {@link SectionAbstract }
     *     
     */
    public void setSectionAbstract(SectionAbstract value) {
        this.sectionAbstract = value;
    }

    /**
     * Gets the value of the chairEditor property.
     * 
     * @return
     *     possible object is
     *     {@link ChairEditor }
     *     
     */
    public ChairEditor getChairEditor() {
        return chairEditor;
    }

    /**
     * Sets the value of the chairEditor property.
     * 
     * @param value
     *     allowed object is
     *     {@link ChairEditor }
     *     
     */
    public void setChairEditor(ChairEditor value) {
        this.chairEditor = value;
    }

    /**
     * Gets the value of the fulltext property.
     * 
     * @return
     *     possible object is
     *     {@link Fulltext }
     *     
     */
    public Fulltext getFulltext() {
        return fulltext;
    }

    /**
     * Sets the value of the fulltext property.
     * 
     * @param value
     *     allowed object is
     *     {@link Fulltext }
     *     
     */
    public void setFulltext(Fulltext value) {
        this.fulltext = value;
    }

    /**
     * Gets the value of the articleRec property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the articleRec property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getArticleRec().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ArticleRec }
     * 
     * 
     */
    public List<ArticleRec> getArticleRec() {
        if (articleRec == null) {
            articleRec = new ArrayList<ArticleRec>();
        }
        return this.articleRec;
    }

}
