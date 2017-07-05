// Collapsible Panels
$(document).on('click resize', '.panel-heading span.clickable', function(event) {
    var $this = $(this);
    if (!$this.hasClass('panel-collapsed') || !$this.is(':visible')) {
        $this.parents('.panel').find('.panel-body').slideUp();
        $this.addClass('panel-collapsed');
        $this.find('i').removeClass('glyphicon-chevron-up').addClass('glyphicon-chevron-down');
    } else {
        $this.parents('.panel').find('.panel-body').slideDown();
        $this.removeClass('panel-collapsed');
        $this.find('i').removeClass('glyphicon-chevron-down').addClass('glyphicon-chevron-up');
    }
});