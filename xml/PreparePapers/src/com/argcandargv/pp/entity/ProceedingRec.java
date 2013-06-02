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
    "procId",
    "acronym",
    "procDesc",
    "conferenceNumber",
    "procClass",
    "procTitle",
    "procSubtitle",
    "procVolumeNo",
    "isbn",
    "isbn13",
    "issn",
    "eissn",
    "copyrightYear",
    "publicationDate",
    "pages",
    "plusPages",
    "offerings",
    "price",
    "otherSource",
    "_abstract",
    "publisher",
    "sponsorRec",
    "incoopRec",
    "categories",
    "generalTerms",
    "chairEditor",
    "frontMatter",
    "backMatter",
    "cmsConfId",
    "msfOffering",
    "acceptanceRates",
    "coverImages",
    "ccc",
    "printOnDemand"
})
@XmlRootElement(name = "proceeding_rec")
public class ProceedingRec {

    @XmlElement(name = "proc_id")
    protected String procId;
    @XmlElement(required = true)
    protected String acronym;
    @XmlElement(name = "proc_desc", required = true)
    protected String procDesc;
    @XmlElement(name = "conference_number")
    protected String conferenceNumber;
    @XmlElement(name = "proc_class", required = true)
    protected String procClass;
    @XmlElement(name = "proc_title", required = true)
    protected String procTitle;
    @XmlElement(name = "proc_subtitle")
    protected String procSubtitle;
    @XmlElement(name = "proc_volume_no")
    protected String procVolumeNo;
    protected String isbn;
    protected String isbn13;
    protected String issn;
    protected String eissn;
    @XmlElement(name = "copyright_year", required = true)
    protected String copyrightYear;
    @XmlElement(name = "publication_date", required = true)
    protected String publicationDate;
    @XmlElement(required = true)
    protected String pages;
    @XmlElement(name = "plus_pages")
    protected String plusPages;
    protected Offerings offerings;
    protected String price;
    @XmlElement(name = "other_source")
    protected String otherSource;
    @XmlElement(name = "abstract")
    protected Abstract _abstract;
    @XmlElement(required = true)
    protected Publisher publisher;
    @XmlElement(name = "sponsor_rec")
    protected SponsorRec sponsorRec;
    @XmlElement(name = "incoop_rec")
    protected IncoopRec incoopRec;
    @XmlElement(required = true)
    protected Categories categories;
    @XmlElement(name = "general_terms")
    protected GeneralTerms generalTerms;
    @XmlElement(name = "chair_editor", required = true)
    protected ChairEditor chairEditor;
    @XmlElement(name = "front_matter")
    protected FrontMatter frontMatter;
    @XmlElement(name = "back_matter")
    protected BackMatter backMatter;
    @XmlElement(name = "cms_conf_id")
    protected String cmsConfId;
    @XmlElement(name = "msf_offering")
    protected List<MsfOffering> msfOffering;
    @XmlElement(name = "acceptance_rates")
    protected AcceptanceRates acceptanceRates;
    @XmlElement(name = "cover_images")
    protected CoverImages coverImages;
    protected Ccc ccc;
    @XmlElement(name = "print_on_demand")
    protected PrintOnDemand printOnDemand;

    /**
     * Gets the value of the procId property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getProcId() {
        return procId;
    }

    /**
     * Sets the value of the procId property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setProcId(String value) {
        this.procId = value;
    }

    /**
     * Gets the value of the acronym property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getAcronym() {
        return acronym;
    }

    /**
     * Sets the value of the acronym property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setAcronym(String value) {
        this.acronym = value;
    }

    /**
     * Gets the value of the procDesc property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getProcDesc() {
        return procDesc;
    }

    /**
     * Sets the value of the procDesc property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setProcDesc(String value) {
        this.procDesc = value;
    }

    /**
     * Gets the value of the conferenceNumber property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getConferenceNumber() {
        return conferenceNumber;
    }

    /**
     * Sets the value of the conferenceNumber property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setConferenceNumber(String value) {
        this.conferenceNumber = value;
    }

    /**
     * Gets the value of the procClass property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getProcClass() {
        return procClass;
    }

    /**
     * Sets the value of the procClass property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setProcClass(String value) {
        this.procClass = value;
    }

    /**
     * Gets the value of the procTitle property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getProcTitle() {
        return procTitle;
    }

    /**
     * Sets the value of the procTitle property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setProcTitle(String value) {
        this.procTitle = value;
    }

    /**
     * Gets the value of the procSubtitle property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getProcSubtitle() {
        return procSubtitle;
    }

    /**
     * Sets the value of the procSubtitle property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setProcSubtitle(String value) {
        this.procSubtitle = value;
    }

    /**
     * Gets the value of the procVolumeNo property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getProcVolumeNo() {
        return procVolumeNo;
    }

    /**
     * Sets the value of the procVolumeNo property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setProcVolumeNo(String value) {
        this.procVolumeNo = value;
    }

    /**
     * Gets the value of the isbn property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIsbn() {
        return isbn;
    }

    /**
     * Sets the value of the isbn property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIsbn(String value) {
        this.isbn = value;
    }

    /**
     * Gets the value of the isbn13 property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIsbn13() {
        return isbn13;
    }

    /**
     * Sets the value of the isbn13 property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIsbn13(String value) {
        this.isbn13 = value;
    }

    /**
     * Gets the value of the issn property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIssn() {
        return issn;
    }

    /**
     * Sets the value of the issn property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIssn(String value) {
        this.issn = value;
    }

    /**
     * Gets the value of the eissn property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getEissn() {
        return eissn;
    }

    /**
     * Sets the value of the eissn property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setEissn(String value) {
        this.eissn = value;
    }

    /**
     * Gets the value of the copyrightYear property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCopyrightYear() {
        return copyrightYear;
    }

    /**
     * Sets the value of the copyrightYear property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCopyrightYear(String value) {
        this.copyrightYear = value;
    }

    /**
     * Gets the value of the publicationDate property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPublicationDate() {
        return publicationDate;
    }

    /**
     * Sets the value of the publicationDate property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPublicationDate(String value) {
        this.publicationDate = value;
    }

    /**
     * Gets the value of the pages property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPages() {
        return pages;
    }

    /**
     * Sets the value of the pages property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPages(String value) {
        this.pages = value;
    }

    /**
     * Gets the value of the plusPages property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPlusPages() {
        return plusPages;
    }

    /**
     * Sets the value of the plusPages property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPlusPages(String value) {
        this.plusPages = value;
    }

    /**
     * Gets the value of the offerings property.
     * 
     * @return
     *     possible object is
     *     {@link Offerings }
     *     
     */
    public Offerings getOfferings() {
        return offerings;
    }

    /**
     * Sets the value of the offerings property.
     * 
     * @param value
     *     allowed object is
     *     {@link Offerings }
     *     
     */
    public void setOfferings(Offerings value) {
        this.offerings = value;
    }

    /**
     * Gets the value of the price property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getPrice() {
        return price;
    }

    /**
     * Sets the value of the price property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setPrice(String value) {
        this.price = value;
    }

    /**
     * Gets the value of the otherSource property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getOtherSource() {
        return otherSource;
    }

    /**
     * Sets the value of the otherSource property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setOtherSource(String value) {
        this.otherSource = value;
    }

    /**
     * Gets the value of the abstract property.
     * 
     * @return
     *     possible object is
     *     {@link Abstract }
     *     
     */
    public Abstract getAbstract() {
        return _abstract;
    }

    /**
     * Sets the value of the abstract property.
     * 
     * @param value
     *     allowed object is
     *     {@link Abstract }
     *     
     */
    public void setAbstract(Abstract value) {
        this._abstract = value;
    }

    /**
     * Gets the value of the publisher property.
     * 
     * @return
     *     possible object is
     *     {@link Publisher }
     *     
     */
    public Publisher getPublisher() {
        return publisher;
    }

    /**
     * Sets the value of the publisher property.
     * 
     * @param value
     *     allowed object is
     *     {@link Publisher }
     *     
     */
    public void setPublisher(Publisher value) {
        this.publisher = value;
    }

    /**
     * Gets the value of the sponsorRec property.
     * 
     * @return
     *     possible object is
     *     {@link SponsorRec }
     *     
     */
    public SponsorRec getSponsorRec() {
        return sponsorRec;
    }

    /**
     * Sets the value of the sponsorRec property.
     * 
     * @param value
     *     allowed object is
     *     {@link SponsorRec }
     *     
     */
    public void setSponsorRec(SponsorRec value) {
        this.sponsorRec = value;
    }

    /**
     * Gets the value of the incoopRec property.
     * 
     * @return
     *     possible object is
     *     {@link IncoopRec }
     *     
     */
    public IncoopRec getIncoopRec() {
        return incoopRec;
    }

    /**
     * Sets the value of the incoopRec property.
     * 
     * @param value
     *     allowed object is
     *     {@link IncoopRec }
     *     
     */
    public void setIncoopRec(IncoopRec value) {
        this.incoopRec = value;
    }

    /**
     * Gets the value of the categories property.
     * 
     * @return
     *     possible object is
     *     {@link Categories }
     *     
     */
    public Categories getCategories() {
        return categories;
    }

    /**
     * Sets the value of the categories property.
     * 
     * @param value
     *     allowed object is
     *     {@link Categories }
     *     
     */
    public void setCategories(Categories value) {
        this.categories = value;
    }

    /**
     * Gets the value of the generalTerms property.
     * 
     * @return
     *     possible object is
     *     {@link GeneralTerms }
     *     
     */
    public GeneralTerms getGeneralTerms() {
        return generalTerms;
    }

    /**
     * Sets the value of the generalTerms property.
     * 
     * @param value
     *     allowed object is
     *     {@link GeneralTerms }
     *     
     */
    public void setGeneralTerms(GeneralTerms value) {
        this.generalTerms = value;
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
     * Gets the value of the frontMatter property.
     * 
     * @return
     *     possible object is
     *     {@link FrontMatter }
     *     
     */
    public FrontMatter getFrontMatter() {
        return frontMatter;
    }

    /**
     * Sets the value of the frontMatter property.
     * 
     * @param value
     *     allowed object is
     *     {@link FrontMatter }
     *     
     */
    public void setFrontMatter(FrontMatter value) {
        this.frontMatter = value;
    }

    /**
     * Gets the value of the backMatter property.
     * 
     * @return
     *     possible object is
     *     {@link BackMatter }
     *     
     */
    public BackMatter getBackMatter() {
        return backMatter;
    }

    /**
     * Sets the value of the backMatter property.
     * 
     * @param value
     *     allowed object is
     *     {@link BackMatter }
     *     
     */
    public void setBackMatter(BackMatter value) {
        this.backMatter = value;
    }

    /**
     * Gets the value of the cmsConfId property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCmsConfId() {
        return cmsConfId;
    }

    /**
     * Sets the value of the cmsConfId property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCmsConfId(String value) {
        this.cmsConfId = value;
    }

    /**
     * Gets the value of the msfOffering property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the msfOffering property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getMsfOffering().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link MsfOffering }
     * 
     * 
     */
    public List<MsfOffering> getMsfOffering() {
        if (msfOffering == null) {
            msfOffering = new ArrayList<MsfOffering>();
        }
        return this.msfOffering;
    }

    /**
     * Gets the value of the acceptanceRates property.
     * 
     * @return
     *     possible object is
     *     {@link AcceptanceRates }
     *     
     */
    public AcceptanceRates getAcceptanceRates() {
        return acceptanceRates;
    }

    /**
     * Sets the value of the acceptanceRates property.
     * 
     * @param value
     *     allowed object is
     *     {@link AcceptanceRates }
     *     
     */
    public void setAcceptanceRates(AcceptanceRates value) {
        this.acceptanceRates = value;
    }

    /**
     * Gets the value of the coverImages property.
     * 
     * @return
     *     possible object is
     *     {@link CoverImages }
     *     
     */
    public CoverImages getCoverImages() {
        return coverImages;
    }

    /**
     * Sets the value of the coverImages property.
     * 
     * @param value
     *     allowed object is
     *     {@link CoverImages }
     *     
     */
    public void setCoverImages(CoverImages value) {
        this.coverImages = value;
    }

    /**
     * Gets the value of the ccc property.
     * 
     * @return
     *     possible object is
     *     {@link Ccc }
     *     
     */
    public Ccc getCcc() {
        return ccc;
    }

    /**
     * Sets the value of the ccc property.
     * 
     * @param value
     *     allowed object is
     *     {@link Ccc }
     *     
     */
    public void setCcc(Ccc value) {
        this.ccc = value;
    }

    /**
     * Gets the value of the printOnDemand property.
     * 
     * @return
     *     possible object is
     *     {@link PrintOnDemand }
     *     
     */
    public PrintOnDemand getPrintOnDemand() {
        return printOnDemand;
    }

    /**
     * Sets the value of the printOnDemand property.
     * 
     * @param value
     *     allowed object is
     *     {@link PrintOnDemand }
     *     
     */
    public void setPrintOnDemand(PrintOnDemand value) {
        this.printOnDemand = value;
    }

}
