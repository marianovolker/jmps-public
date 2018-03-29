
(function()
{
	window.SectionLoadOrderControler=new function SectionLoadOrderControler()
	{
		var callbackObject=new Array();
		this.Notify=function()
		{
			while ((callbackObject.length > 0) && ((callbackObject[0])() != false))
			{
				callbackObject.shift();
			}
			return this;
		};
		this.Subscribe=function(callback)
		{
			callbackObject.push(callback);
			return this;
		};
		this.Clear=function()
		{
			callbackObject.length=0;
			return this;
		};		
	};
})();

function TextField(name, styleClass, parent, text, removeOverlaps, overlapMargin)
{
    // loadingAttributes
	this.isLoadComplete		= false;
	this.FinishLoad			= null;

	this.fixedSectionClass	= 'fixedText';
	this.freeSectionClass	= 'freeText';
	this.isFreeSection		= false;

    this.waitCount 			= 0;
	this.delay_amount		= 100;
	this.delay_count		= 10;

	this.name				= name;
	this.styleClass			= styleClass;

	if (parent != null)
		this.parent				= parent;
	else
		this			=	'body';

	this.text				= text;
	this.removeOverlaps		= removeOverlaps;
	this.overlapMargin		= overlapMargin;

	this.x					= 0;
	this.y					= 0;
	this.positioning		= 0;

	this.visibility			= true;
	this.eraseLine			= false;
	this.background_color	= null;
	this.color				= null;
	this.underline			= false;
	this.scroll				= false;

	this.sectionEraser 		= null;
	this.MainDiv		  	= null;
}

TextField.prototype.Init = function()
{
	if( !this.sectionEraser && this.eraseLine )
	{
		this.sectionEraser = 
		$('<div style=\"position:absolute; top:0px; left:0px; width:100%; height:0%; visibility:hidden;\"/>')
		.appendTo('body');
	}

	if( !this.MainDiv )
	{
		this.MainDiv = 
		$('<div id=\"'+this.name+'\" style=\"position:absolute; overflow:hidden; white-space:pre; top:0px; left:0px; max-width:auto; max-height:auto; display:none; visibility:hidden;\"/>')
		.append(
			$('<div class=\"'+this.styleClass+'\" style=\"position:relative; top:0px; left:0px;'+
				((this.underline)?(' text-decoration:underline;'):(''))+
				((this.background_color)?(' background-color:'+this.background_color+';'):(''))+
				((this.color)?(' color:'+this.color+';'):(''))+		
			'\"/>')
			.append(this.text)
		)
		.appendTo('body');
	}
}

TextField.prototype.UpdatePositioning = function()
{
	if (this.MainDiv)
	{
		var positioning = null;
		if (this.scroll)
		{
			positioning = {'top':this.y+'px','left':'0px','right':'auto','width':'100%','max-height':this.MainDiv.height()+'px','display':'inline'};
		}
		else
		{
			switch(this.positioning)
			{
			case 2:
				positioning = {'top':this.y+'px','left':((this.MainDiv.parent().width()-this.MainDiv.width())/2)+'px','right':'auto','max-width':this.MainDiv.width()+'px','max-height':this.MainDiv.height()+'px','display':'inline'};
				break;
			case 3:
				positioning = {'top':this.y+'px','left':((this.x>this.MainDiv.width())?(this.x-this.MainDiv.width()):(0))+'px','right':'auto','max-width':this.MainDiv.width()+'px','max-height':this.MainDiv.height()+'px','display':'inline'};
				break;
			case 0:
			case 1:
			default:
				positioning = {'top':this.y+'px','left':this.x+'px','right':'auto','max-width':this.MainDiv.width()+'px','max-height':this.MainDiv.height()+'px','display':'inline'};
				break;
			}
		}
		this.MainDiv.css(positioning);

		if (this.sectionEraser)
		{
			this.sectionEraser.css({'top':this.MainDiv.css('top'),'height':this.MainDiv.height()+'px','background-color':this.MainDiv.children().css('background-color')});
		}	
	}
}

TextField.prototype.Show = function()
{
	if (this.MainDiv)
	{
		this.MainDiv.remove();
		if (!this.isFreeSection)
		{
			this.ErasePrevious();
		}
		if (this.scroll)
		{
			this.MainDiv.children().wrapInner('<div style="position:relative; overflow:hidden; width:100%; height:100%; margin:0px; padding:0px; border:0px;"><marquee behavior="scroll" scrolldelay="5" scrollamount="2" direction="left" width="100%" loop="true"' + ((this.underline) ? (' style="text-decoration: underline;"') : ('')) + '></marquee></div>');
		}
		this.MainDiv.appendTo(this.parent);
		if (this.visibility)
		{
			var visibilityAttribute = {'visibility':'visible'};
			if (this.sectionEraser)
			{
				this.sectionEraser.css(visibilityAttribute);
			}
			this.MainDiv.css(visibilityAttribute);
		}
	}
}

// Setters
TextField.prototype.SetY  				= function (y)					{	this.y 					= y;				return this;	}
TextField.prototype.SetX 				= function (x)					{   this.x 					= x;				return this;	}
TextField.prototype.SetVisibility 		= function (visibility)			{   this.visibility 		= visibility;		return this;	}
TextField.prototype.SetEraseLine 		= function (eraseLine)			{   this.eraseLine 			= eraseLine;		return this;	}
TextField.prototype.SetBackgroundColor	= function (background_color)	{   this.background_color 	= background_color;	return this;	}
TextField.prototype.SetColor			= function (color)				{   this.color 				= color;			return this;	}
TextField.prototype.SetUnderLine 		= function (underline)			{   this.underline 			= underline;		return this;	}
TextField.prototype.SetScroll			= function (scroll)				{	this.scroll				= scroll;			return this;	}

TextField.prototype.EraseLineDivIsLoaded = function ()
{
	if( this.sectionEraser )
	{
		if( (this.text.length < 1) || (this.sectionEraser.height() > 0) || (this.delay_count < 1) )
		{
			return true;
		}
		return false;
	}

	return true;
}

TextField.prototype.MainDivIsLoaded = function ()
{
	if( (this.MainDiv) && ( (this.text.length < 1) || (this.MainDiv.width() > 0) || (this.delay_count < 1) ) )
	{
		return true;
	}

	return false;
}

TextField.prototype.SetRetryTimeOut = function ()
{
	var self = this;

	this.delay_count--;

	setTimeout
		(
			function()
			{
				self.Core();
			}
			, 
			this.delay_amount
		);
}

TextField.prototype.SetAligmentCenter = function ()
{
	this.positioning = 2;
	return this;
}

TextField.prototype.SetAligmentRigth = function ()
{
	this.positioning = 3;
	return this;
}

TextField.prototype.SetAligmentLeft = function ()
{
	this.positioning = 1;
	return this;
}

TextField.prototype.SetAligmentNone = function ()
{
	this.positioning = 0;
	return this;
}

TextField.prototype.FreeSectionCheck = function ()
{
	var target			= this.sectionEraser||this.MainDiv;
	var removeOverlaps	= (this.removeOverlaps)?(this.removeOverlaps):(false);
	var nMargin			= (this.overlapMargin != null)?(this.overlapMargin):(10);

	if (this.isFreeSection)
	{
		if (target)
		{
			if( removeOverlaps )
			{
				$('.'+this.freeSectionClass).each
				(
					function(index)
					{
						var w1=target.outerWidth()-(2*nMargin);
						var h1=target.outerHeight()-(2*nMargin);
						var x1=target.offset().left+nMargin;
						var y1=target.offset().top+nMargin;
						var w2=$(this).outerWidth();
						var h2=$(this).outerHeight();
						var x2=$(this).offset().left;
						var y2=$(this).offset().top;

						if( ((x1+w1)>x2)&&((y1+h1)>y2)&&((x2+w2)>x1)&&((y2+h2)>y1) )
						{
							$(this).remove();
						}
					}
				);

				if(this.sectionEraser)
				{
					$(this.sectionEraser).remove();
					delete this.sectionEraser;
					this.sectionEraser=null;
				}
			}
		}
		if (this.MainDiv)
		{
			this.MainDiv.addClass(this.freeSectionClass);
		}
	}
	else
	{
		if (this.MainDiv)
		{
			this.MainDiv.addClass(this.fixedSectionClass);
		}
	}
}

TextField.prototype.LoadChecker = function()
{
	if (this.isLoadComplete && this.FinishLoad)
	{
		this.FinishLoad();
	}
	return this.isLoadComplete;
}

TextField.prototype.SubscribeLoadChecker = function()
{
	var self = this;
	SectionLoadOrderControler.Subscribe
	(
		function()
		{
			return self.LoadChecker();
		}
	);
}

TextField.prototype.NotifyLoadChecker = function()
{
	this.isLoadComplete = true;
	if (this.visibility)
	{
		this.FinishLoad = this.CompleteDisplay;
	}
	else
	{
		this.FinishLoad = this.ErasePrevious;
	}
	SectionLoadOrderControler.Notify();
}

TextField.prototype.ErasePrevious = function()
{

	$( this.parent + '>' + '#'+this.name+'.'+this.fixedSectionClass).remove();
}

TextField.prototype.CompleteDisplay = function()
{
	this.FreeSectionCheck();
	this.Show();
}

TextField.prototype.Core = function()
{
	if ( this.MainDivIsLoaded() && this.EraseLineDivIsLoaded() )
	{
		this.UpdatePositioning();
		this.NotifyLoadChecker();
	}
	else
	{
		this.SetRetryTimeOut();
	}
}

TextField.prototype.ImageLoadCheck = function()
{
	var images = $('img[_src]', this.MainDiv.get(0)) .get();
	var self = this;
	if( images.length > 0 )
	{
		self.waitCount = images.length;
		
		$(images).each
		(
			function(i)
			{
				var imgSrc = $(this).attr('_src');
				
				$(this).removeAttr('_src');
				
				$(this).load
				(
					function()
					{
						self.waitCount--;
						if( self.waitCount < 1 )
						{
							self.Core();
						}
					}
				);
				
				$(this).error
				(
					function()
					{
						self.waitCount--;
						if( self.waitCount < 1 )
						{
							self.Core();
						}
					}
				);
				
				$(this).attr('src', imgSrc);
			}
		 );
	}
	else
	{
		self.Core();
	}
}

TextField.prototype.Build = function (freeSection)
{
	this.isFreeSection = ((freeSection)?(true):(false));
	this.SubscribeLoadChecker();
	if (this.visibility)
	{
		this.Init();
		this.ImageLoadCheck();
	}
	else if (!this.isFreeSection)
	{
		this.NotifyLoadChecker();
	}
	return this;
}

function GraphicSegment(name, parent, content, x, y, nWidth, nHeight)
{
    // loadingAttributes
	this.isLoadComplete		= false;
	this.FinishLoad			= null;

	this.fixedSectionClass	= 'fixedSegment';
	this.freeSectionClass	= 'freeSegment';
	this.isFreeSection		= false;

	this.waitCount 			= 0;
	this.delay_amount		= 100;
	this.delay_count		= 10;

	this.name				= name;
	this.parent				= parent;
	this.content			= content;

	this.x					= x;
	this.y					= y;
	this.nWidth				= nWidth;
	this.nHeight			= nHeight;

	this.visibility			= true;

	this.MainDiv		  	= null;
}

GraphicSegment.prototype.Init = function()
{
	if( !this.MainDiv )
	{
		this.MainDiv = 
		$('<div id=\"'+this.name+'\" style=\"position:absolute; top:'+this.y+'px; left:'+this.x+'px; width:'+this.nWidth+'px; height:'+this.nHeight+'px; display:none; visibility:visible;\"/>')
		.append(this.content)
		.appendTo('body');
	}
}

GraphicSegment.prototype.Show = function()
{
	if (this.MainDiv)
	{
		this.MainDiv.remove();
		if (!this.isFreeSection)
		{
			this.ErasePrevious();
		}
		this.MainDiv.appendTo(this.parent);
		if (this.visibility)
		{
			var visibilityAttribute = {'display':'inline'};
			this.MainDiv.css(visibilityAttribute);
		}
	}
}

// Setters
GraphicSegment.prototype.SetY			= function (y)					{	this.y 					= y;				return this;	}
GraphicSegment.prototype.SetX			= function (x)					{   this.x 					= x;				return this;	}
GraphicSegment.prototype.SetVisibility  = function (visibility)			{   this.visibility 		= visibility;		return this;	}

GraphicSegment.prototype.MainDivIsLoaded = function ()
{
	if( (this.MainDiv) && ( (this.content.length < 1) || (this.MainDiv.width() > 0) || (this.delay_count < 1) ) )
	{
		return true;
	}

	return false;
}

GraphicSegment.prototype.SetRetryTimeOut = function ()
{
	var self = this;

	this.delay_count--;

	setTimeout
		(
			function()
			{
				self.Core();
			}
			, 
			this.delay_amount
		);
}

GraphicSegment.prototype.FreeSectionCheck = function ()
{
	if (this.MainDiv)
	{
		if (this.isFreeSection)
		{
			this.MainDiv.addClass(this.freeSectionClass);
		}
		else
		{
			this.MainDiv.addClass(this.fixedSectionClass);
		}
	}
}

GraphicSegment.prototype.LoadChecker = function()
{
	if (this.isLoadComplete)
	{
		this.FinishLoad();
	}
	return this.isLoadComplete;
}

GraphicSegment.prototype.SubscribeLoadChecker = function()
{
	var self = this;
	SectionLoadOrderControler.Subscribe
		(
			function()
			{
				return self.LoadChecker();
			}
		);
}

GraphicSegment.prototype.NotifyLoadChecker = function()
{
	this.isLoadComplete = true;
	if (this.visibility)
	{
		this.FinishLoad = this.CompleteDisplay;
	}
	else
	{
		this.FinishLoad = this.ErasePrevious;
	}
	SectionLoadOrderControler.Notify();
}

GraphicSegment.prototype.ErasePrevious = function()
{
	$('#'+this.name+'.'+this.fixedSectionClass).remove();
}

GraphicSegment.prototype.CompleteDisplay = function()
{
	this.FreeSectionCheck();
	this.Show();
}

GraphicSegment.prototype.Core = function()
{
	if ( this.MainDivIsLoaded() )
	{
		this.NotifyLoadChecker();
	}
	else
	{
		this.SetRetryTimeOut();
	}
}

GraphicSegment.prototype.ImageLoadCheck = function()
{
	var images = $('img[_src]', this.MainDiv.get(0)) .get();
	var self = this;
	if( images.length > 0 )
	{
		self.waitCount = images.length;
		
		$(images).each
		(
			function(i)
			{
				var imgSrc = $(this).attr('_src');
				
				$(this).removeAttr('_src');
				
				$(this).load
				(
					function()
					{
						self.waitCount--;
						if( self.waitCount < 1 )
						{
							self.Core();
						}
					}
				);
				
				$(this).error
				(
					function()
					{
						self.waitCount--;
						if( self.waitCount < 1 )
						{
							self.Core();
						}
					}
				);
				
				$(this).attr('src', imgSrc);
			}
		 );
	}
	else
	{
		self.Core();
	}
}

GraphicSegment.prototype.Build = function (freeSection)
{
	this.isFreeSection = ((freeSection)?(true):(false));
	this.SubscribeLoadChecker();
	if (this.visibility)
	{
		this.Init();
		this.ImageLoadCheck();
	}
	else if (!this.isFreeSection)
	{
		this.NotifyLoadChecker();
	}
	return this;
}
