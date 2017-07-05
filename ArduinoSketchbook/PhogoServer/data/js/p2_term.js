"use strict";
/* JQuery Terminal: http://terminal.jcubic.pl/api_reference.php */
/* includes sprintf() */
function horizontal(topics, c, b) {
    // get the longest word
    const longest = topics.reduce((a, b) => {
        return (a.length > b.length) ? a : b
    }).length;
    // determine maximum words per line
    const cols = Math.floor((c + b) / (longest + b));
    // sort topics
    topics = topics.sort();
    // split the array
    const lines = topics.map(function(item, index) {
        return index % cols === 0 ? topics.slice(index, index + cols) : null;
    }).filter(function(item) {
        return item;
    });
    const fmt = sprintf("%%-%ds", longest + b); // format: longest word + blanks
    const lfmt = sprintf("%%-%ds", longest); // format for last in line: no blanks
    let line = '';
    let help = [];
    for (let g = 0; g < lines.length; ++g) {
        let group = lines[g];
        while (group.length > 1) { // while there's more than 1 left
            line += sprintf(fmt, group.shift()); // pop from the front and print
        }
        line += sprintf(lfmt, group.shift()); // print last
        help.push(s); // store
        line = ''; // reset
    }
    // join with newline
    help = help.join('\n');
    return help;
}

function vertical(topics, c, b) {
    // get the longest word
    const longest = topics.reduce((a, b) => {
        return (a.length > b.length) ? a : b
    }).length;
    // sort topics
    topics = topics.sort();
    // number of columns
    const cols = Math.floor((c + b) / (longest + b));
    // number of rows
    let rows = Math.ceil(topics.length / cols);
    // store the lines
    let lines = {};
    for (let i = 0; i < topics.length; ++i) {
        const ind = i % rows;
        lines[ind] = lines[ind] || [];
        lines[ind].push(topics[i]);
    }
    const fmt = sprintf("%%-%ds", longest + b); // format: longest word + blanks
    const lfmt = sprintf("%%-%ds", longest); // format for last in line: no blanks
    let s = '';
    let help = [];
    // get the keys sorted as numbers
    const keys = Object.keys(lines).sort((a, b) => {
        return parseInt(a) - parseInt(b);
    });
    for (let g = 0; g < keys.length; ++g) {
        let group = lines[keys[g]];
        while (group.length > 1) { // while there's more than 1 left
            s += sprintf(fmt, group.shift()); // pop from the front and print
        }
        s += sprintf(lfmt, group.shift()); // print last
        help.push(s); // store
        s = ''; // reset
    }
    help = help.join('\n'); // join with newline
    return help;
}

function help(topic, columns, blanks) {
    console.warn('args', arguments);
    const HELP = {
        help: 'Display help.',
        set_theme: 'Change the theme of the site to light/dark. ', // + (()=>{return "Current: " + window.location})(),
        get default() {
            let topics = [],
                c = columns,
                b = blanks,
                longest = 0;
            for (let i in this) {
                if (this.hasOwnProperty(i) && i !== 'default') {
                    topics.push(i);
                }
            }
            let h = vertical(topics, columns, blanks);
            h += "\n\nType 'help <command>' for a more detailed help";
            return h;
        }
    }
    return HELP[topic];
}

$(function() {
    const terminal_commands = {
        help: function() {
            if (arguments.length > 0) {
                this.echo(help(arguments[0].toLowerCase()));
            } else {
                this.echo(help('default', this.cols(), 4));
            }
        }
    };
    document.terminal = $('#terminal').terminal(terminal_commands, {
        greetings: "Phogo version 2\n\nType 'help' to list available commands",
        prompt: '> ',
        checkArity: false
    });
});