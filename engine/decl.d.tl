global record Array
    new: function (integer) : {integer}
end

global record SerpentOptions
	indent: string
	comment: boolean | integer
	sortkeys: boolean | function
	sparse: boolean
	compact: boolean
	fatal: boolean
	fixradix: boolean
	nocode: boolean
	nohuge: boolean
	maxlevel: integer
	maxnum: integer
	maxlength: integer
	metatostring: boolean
	numformat: string
	valignore: table
	keyallow: table
	keyignore: table
	valtypeignore: table
	custom: function
	name: string
end

global record serpent
	line: function(any, _?:SerpentOptions): string
	dump: function(any, _?:SerpentOptions): string
	block: function(any, _?:SerpentOptions): string
end

global type ResourceId = integer
global type cache_image = function(name: string) : ResourceId
