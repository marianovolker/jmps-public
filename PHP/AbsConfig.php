<?php

  require_once("Site.php");

  /**
   * Config
   *
   * Base class to parse a configuration file saved as ini file type or by a database/table.
   *
   * @author   Mariano Volker <marianovolker@yahoo.com.ar>
   * @package  Security Module
   */
  abstract class AbsConfig
  {
    /**
     * Data member to store the entire content of config in the form as "[section][key] = value"
     */
    protected $internalStorageArray;
    
    /**
     * Data member to store the table or file"
     */
    protected $entity;
    
    /**
     * Post pattern for update action...
     */
    protected $postPatternUpdateAction;
    
    

    /**
     * isPOSTActionForUpdate
     *
     * Verify if a POST update action is pending to process. 
     */    
    public function isPOSTActionForUpdate()
    {
      return isset( $_POST[$this->postPatternUpdateAction] );
    }
    
    /**
     * setPOSTActionForUpdate
     *
     * Set a POST update action to this object.
     */
    public function setPOSTActionForUpdate($postAction)
    {
      $this->postPatternUpdateAction = $postAction;
    }
    
    /**
     * getValue
     *
     * Obtain the value of the key inside the section.
     */
    public function getValue( $section, $key, $default="not set" )
    {
      $section=strtolower($section);
      $key=strtolower($key);
      return ( (isset($this->internalStorageArray[$section][$key])) ? ($this->internalStorageArray[$section][$key]) : ($default) );
    }
    
    /**
     * getEntity
     *
     * Obtain the entity where the values are stored.
     * This value could be a file or a table in a DB. 
     */
    public function getEntity( )
    {
      return $this->entity;
    }
    
    /**
     * getValuesAsArray
     *
     * Obtain all values as an array.
     */
    public function getValuesAsArray( )
    {
      return $this->internalStorageArray;
    }
    
    /**
     * setValuesToArray
     *
     * Set all values in internal array.
     */
    public function setValuesToArray( $values )
    {
      $this->internalStorageArray = $values;
    }
    
    /**
     * save
     *
     * Store internal values into media selected (DB or File).
     */
    abstract public function save();
  }
?>
