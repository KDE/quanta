<!DOCTYPE tags>
<tags>
	<tag name="ArrayIterator::current" type="function" returnType="mixed" version="">
	</tag>
	<tag name="ArrayIterator::key" type="function" returnType="mixed" version="">
	</tag>
	<tag name="ArrayIterator::next" type="function" returnType="void" version="">
	</tag>
	<tag name="ArrayIterator::rewind" type="function" returnType="void" version="">
	</tag>
	<tag name="ArrayIterator::seek" type="function" returnType="void" version="">
		<attr name="position" type="int" status="required"></attr>
	</tag>
	<tag name="ArrayIterator::valid" type="function" returnType="bool" version="">
	</tag>
	<tag name="ArrayObject::append" type="function" returnType="void" version="">
		<attr name="newval" type="mixed" status="required"></attr>
	</tag>
	<tag name="ArrayObject::__construct" type="function" returnType="void" version="">
		<attr name="input" type="mixed" status="required"></attr>
	</tag>
	<tag name="ArrayObject::count" type="function" returnType="int" version="">
	</tag>
	<tag name="ArrayObject::getIterator" type="function" returnType="ArrayIterator" version="">
	</tag>
	<tag name="ArrayObject::offsetExists" type="function" returnType="bool" version="">
		<attr name="index" type="mixed" status="required"></attr>
	</tag>
	<tag name="ArrayObject::offsetGet" type="function" returnType="bool" version="">
		<attr name="index" type="mixed" status="required"></attr>
	</tag>
	<tag name="ArrayObject::offsetSet" type="function" returnType="void" version="">
		<attr name="index" type="mixed" status="required"></attr>
		<attr name="newval" type="mixed" status="required"></attr>
	</tag>
	<tag name="ArrayObject::offsetUnset" type="function" returnType="void" version="">
		<attr name="index" type="mixed" status="required"></attr>
	</tag>
	<tag name="CachingIterator::hasNext" type="function" returnType="boolean" version="">
	</tag>
	<tag name="CachingIterator::next" type="function" returnType="void" version="">
	</tag>
	<tag name="CachingIterator::rewind" type="function" returnType="void" version="">
	</tag>
	<tag name="CachingIterator::__toString" type="function" returnType="string" version="">
	</tag>
	<tag name="CachingIterator::valid" type="function" returnType="boolean" version="">
	</tag>
	<tag name="CachingRecursiveIterator::getChildren" type="function" returnType="CachingRecursiveIterator" version="">
	</tag>
	<tag name="CachingRecursiveIterator::hasChildren" type="function" returnType="bolean" version="">
	</tag>
	<tag name="DirectoryIterator::__construct" type="function" returnType="void" version="">
		<attr name="path" type="string" status="required"></attr>
	</tag>
	<tag name="DirectoryIterator::current" type="function" returnType="DirectoryIterator" version="">
	</tag>
	<tag name="DirectoryIterator::getATime" type="function" returnType="int" version="">
	</tag>
	<tag name="DirectoryIterator::getCTime" type="function" returnType="int" version="">
	</tag>
	<tag name="DirectoryIterator::getChildren" type="function" returnType="RecursiveDirectoryIterator" version="">
	</tag>
	<tag name="DirectoryIterator::getFilename" type="function" returnType="string" version="">
	</tag>
	<tag name="DirectoryIterator::getGroup" type="function" returnType="int" version="">
	</tag>
	<tag name="DirectoryIterator::getInode" type="function" returnType="int" version="">
	</tag>
	<tag name="DirectoryIterator::getMTime" type="function" returnType="int" version="">
	</tag>
	<tag name="DirectoryIterator::getOwner" type="function" returnType="int" version="">
	</tag>
	<tag name="DirectoryIterator::getPath" type="function" returnType="string" version="">
	</tag>
	<tag name="DirectoryIterator::getPathname" type="function" returnType="string" version="">
	</tag>
	<tag name="DirectoryIterator::getPerms" type="function" returnType="int" version="">
	</tag>
	<tag name="DirectoryIterator::getSize" type="function" returnType="int" version="">
	</tag>
	<tag name="DirectoryIterator::getType" type="function" returnType="string" version="">
	</tag>
	<tag name="DirectoryIterator::isDir" type="function" returnType="bool" version="">
	</tag>
	<tag name="DirectoryIterator::isDot" type="function" returnType="bool" version="">
	</tag>
	<tag name="DirectoryIterator::isExecutable" type="function" returnType="bool" version="">
	</tag>
	<tag name="DirectoryIterator::isFile" type="function" returnType="bool" version="">
	</tag>
	<tag name="DirectoryIterator::isLink" type="function" returnType="bool" version="">
	</tag>
	<tag name="DirectoryIterator::isReadable" type="function" returnType="bool" version="">
	</tag>
	<tag name="DirectoryIterator::isWritable" type="function" returnType="bool" version="">
	</tag>
	<tag name="DirectoryIterator::key" type="function" returnType="string" version="">
	</tag>
	<tag name="DirectoryIterator::next" type="function" returnType="void" version="">
	</tag>
	<tag name="DirectoryIterator::rewind" type="function" returnType="void" version="">
	</tag>
	<tag name="DirectoryIterator::valid" type="function" returnType="string" version="">
	</tag>
	<tag name="FilterIterator::current" type="function" returnType="mixed" version="">
	</tag>
	<tag name="FilterIterator::getInnerIterator" type="function" returnType="Iterator" version="">
	</tag>
	<tag name="FilterIterator::key" type="function" returnType="mixed" version="">
	</tag>
	<tag name="FilterIterator::next" type="function" returnType="void" version="">
	</tag>
	<tag name="FilterIterator::rewind" type="function" returnType="void" version="">
	</tag>
	<tag name="FilterIterator::valid" type="function" returnType="boolean" version="">
	</tag>
	<tag name="LimitIterator::getPosition" type="function" returnType="int" version="">
	</tag>
	<tag name="LimitIterator::next" type="function" returnType="void" version="">
	</tag>
	<tag name="LimitIterator::rewind" type="function" returnType="void" version="">
	</tag>
	<tag name="LimitIterator::seek" type="function" returnType="void" version="">
		<attr name="position" type="int" status="required"></attr>
	</tag>
	<tag name="LimitIterator::valid" type="function" returnType="boolean" version="">
	</tag>
	<tag name="ParentIterator::getChildren" type="function" returnType="ParentIterator" version="">
	</tag>
	<tag name="ParentIterator::hasChildren" type="function" returnType="boolean" version="">
	</tag>
	<tag name="ParentIterator::next" type="function" returnType="void" version="">
	</tag>
	<tag name="ParentIterator::rewind" type="function" returnType="void" version="">
	</tag>
	<tag name="RecursiveDirectoryIterator::getChildren" type="function" returnType="object" version="">
	</tag>
	<tag name="RecursiveDirectoryIterator::hasChildren" type="function" returnType="bool" version="">
		<attr name="allow_links" type="bool" status="optional"></attr>
	</tag>
	<tag name="RecursiveDirectoryIterator::key" type="function" returnType="string" version="">
	</tag>
	<tag name="RecursiveDirectoryIterator::next" type="function" returnType="void" version="">
	</tag>
	<tag name="RecursiveDirectoryIterator::rewind" type="function" returnType="void" version="">
	</tag>
	<tag name="RecursiveIteratorIterator::current" type="function" returnType="mixed" version="">
	</tag>
	<tag name="RecursiveIteratorIterator::getDepth" type="function" returnType="int" version="">
	</tag>
	<tag name="RecursiveIteratorIterator::getSubIterator" type="function" returnType="RecursiveIterator" version="">
	</tag>
	<tag name="RecursiveIteratorIterator::key" type="function" returnType="mixed" version="">
	</tag>
	<tag name="RecursiveIteratorIterator::next" type="function" returnType="void" version="">
	</tag>
	<tag name="RecursiveIteratorIterator::rewind" type="function" returnType="void" version="">
	</tag>
	<tag name="RecursiveIteratorIterator::valid" type="function" returnType="bolean" version="">
	</tag>
	<tag name="SimpleXMLIterator::current" type="function" returnType="mixed" version="">
	</tag>
	<tag name="SimpleXMLIterator::getChildren" type="function" returnType="object" version="">
	</tag>
	<tag name="SimpleXMLIterator::hasChildren" type="function" returnType="bool" version="">
	</tag>
	<tag name="SimpleXMLIterator::key" type="function" returnType="mixed" version="">
	</tag>
	<tag name="SimpleXMLIterator::next" type="function" returnType="void" version="">
	</tag>
	<tag name="SimpleXMLIterator::rewind" type="function" returnType="void" version="">
	</tag>
	<tag name="SimpleXMLIterator::valid" type="function" returnType="bool" version="">
	</tag>
	<tag name="class_implements" type="function" returnType="array" version="PHP 5">
		<attr name="class" type="object" status="required"></attr>
	</tag>
	<tag name="class_parents" type="function" returnType="array" version="PHP 5">
		<attr name="class" type="object" status="required"></attr>
	</tag>
	<tag name="iterator_count" type="function" returnType="int" version="">
		<attr name="iterator" type="IteratorAggregate" status="required"></attr>
	</tag>
	<tag name="iterator_to_array" type="function" returnType="array" version="">
		<attr name="iterator" type="IteratorAggregate" status="required"></attr>
	</tag>
	<tag name="spl_classes" type="function" returnType="array" version="PHP 5">
	</tag>
</tags>