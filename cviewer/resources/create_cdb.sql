--
-- Table structure for `cff_index`
-- that is compatible with the ConnectomeViewer CDB plugin
--

CREATE TABLE IF NOT EXISTS `cff_index` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(100) NOT NULL,
  `species` varchar(50) NOT NULL COMMENT 'binomial nomenclature, connectome wiki conform',
  `legalnotice` varchar(300) default NULL COMMENT 'usage of the dataset',
  `reference` varchar(300) default NULL COMMENT 'reference for this dataset',
  `description` text NOT NULL,
  `nrofnetworks` int(11) NOT NULL,
  `url` varchar(300) default NULL,
  `generator` varchar(50) NOT NULL,
  `cfile_url` varchar(300) NOT NULL,
  `created` date NOT NULL,
  `modified` date NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=10 ;

