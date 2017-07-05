"use strict";
const LS_CODE = "user_code";
/* Debug */
function debug_mode() {
    const LS_MODULES = "brython_modules";
    console.log("Debug mode", !!window.__BRYTHON__.debug);
    if (window.__BRYTHON__ && !!window.__BRYTHON__.debug) {
        let imported = [];
        $("#console_menu_item").removeClass('hidden');
        for (let attr in __BRYTHON__.imported) {
            imported.push(attr);
        }
        imported = imported.sort().join('\n');
        if (window.Features.localstorage) {
            let ls_import = localStorage.getItem(LS_MODULES);
            if (!(ls_import == imported)) {
                localStorage.setItem(LS_MODULES, imported);
                console.log("Imported modules have changed");
                $.post('//' + window.location.hostname + ':5000/brython_modules', imported);
            }
        }
    }
}
/* Feature detection */
/* localStorage detection */
function testLocalStorage() {
    const test = 'zsfG34gW4G'; // no easy collisions
    try {
        if ('localStorage' in window && window['localStorage'] !== null) {
            try {
                localStorage.setItem(test, test);
                localStorage.removeItem(test);
                return true;
            } catch (e) {}
        }
    } catch (e) {}
    return false;
}
/* Web Workers detection */
function testWebWorkers() {
    return window.Worker !== undefined;
}
/* Feature detection: much like Modernizr, but lighter */
$(function() {
    window.Features = {
            localstorage: testLocalStorage(),
            webworkers: testWebWorkers()
        }
        // Run startup functions
    ACEEditor();
    loadBrython();
});
/* Brython */
function loadBrython() {
    const options = {
        debug: 0,
        cache: 'browser',
        // pythonpath: ['py', 'vendor/brython/Lib/site-packages']
    };
    const loader = $('#loading');
    loader.show();
    /* event emitted by app.py */
    loader.on('apploaded', (evt) => {
        loader.hide();
        debug_mode();
    });
    setTimeout(function() {
        brython(options);
    }, 0);
}
/* ACE Editor */
function ACEEditor() {
    let editor = document.editor = ace.edit("editor");
    /* ACE needs this */
    editor.$blockScrolling = Infinity;
    editor.setTheme("ace/theme/monokai");
    editor.setOptions({
        /* 'enableLiveAutocompletion': true, */
        'enableSnippets': false,
        'highlightActiveLine': true,
        'highlightSelectedWord': true,
        'fontSize': '11pt'
    });
    // editor.resize();
    let session = editor.getSession();
    session.setMode("ace/mode/python");
    editor.setValue("");
    if (window.Features.localstorage) {
        const code = localStorage.getItem(LS_CODE);
        if (!!code) {
            editor.setValue(code);
        }
    }
    editor.gotoLine(0);
    editor.focus();
    $('#editor-container').draggable({
        handle: '#editor-panel',
        snap: true,
        containment: "parent"
    }).resizable({
        alsoResize: "#editor"
    }).on("resize", function(event, ui) {
        editor.resize();
    });
};
/* File Options Menu */
function getFileName() {
    let date = new Date();
    let name = 'phogo'
    name += '_' + date.getFullYear();
    name += '_' + ('0' + (date.getMonth() + 1)).slice(-2);
    name += '_' + ('0' + (date.getDate())).slice(-2);
    name += '_' + ('0' + (date.getHours())).slice(-2);
    name += '_' + ('0' + (date.getMinutes())).slice(-2);
    name += '_' + ('0' + (date.getSeconds())).slice(-2);
    name += '.py';
    return name;
}
/* https://thiscouldbebetter.wordpress.com/2012/12/18/loading-editing-and-saving-a-text-file-in-html5-using-javascrip/ */
/* https://www.abeautifulsite.net/whipping-file-inputs-into-shape-with-bootstrap-3 */
function saveCodeAsFile() {
    let textToWrite = document.editor.getValue();
    let textFileAsBlob = new Blob([textToWrite], {
        type: 'text/plain',
        encoding: 'UTF-8'
    });
    let fileNameToSaveAs = getFileName();
    let downloadLink = document.createElement("a");
    downloadLink.download = fileNameToSaveAs;
    downloadLink.innerHTML = "Download File"; // won't be seen
    const blobURL = window.URL.createObjectURL(textFileAsBlob);
    downloadLink.href = blobURL;
    /* remove the clicked anchor after triggering the download */
    downloadLink.onclick = (event) => {
        // window.URL.revokeObjectURL(blobURL);
        document.body.removeChild(event.target)
    };
    downloadLink.style.display = "none";
    document.body.appendChild(downloadLink);
    downloadLink.click();
}
/* Load the user selected file */
$('#fileToLoad').on('change', function(event) {
    loadFileAsText(event.target.files[0]);
    event.target.value = null;
});

function loadFileAsText(file) {
    let fileReader = new FileReader();
    fileReader.onload = function(event) {
        let loadedText = event.target.result;
        document.editor.setValue(loadedText);
    };
    fileReader.readAsText(file, "UTF-8");
}
/* Run the contents of the editor as a script */
function runUserCode(script_id) {
    script_id = script_id || "user-code";
    /* get the script */
    let code = document.getElementById(script_id);
    if (!code) {
        /* or create it if it doesn't exist */
        code = document.createElement("script");
        code.id = script_id;
        document.body.appendChild(code);
    }
    /* make sure it's python */
    code.type = "text/python3";
    /* set the content */
    code.text = document.editor.getValue();
    /* call brython */
    /* keep the same debug level */
    brython({
        debug: window.__BRYTHON__.debug,
        ipy_id: [script_id]
    });
}

function saveCodeToLocalStorage() {
    const code = document.editor.getValue();
    if (window.Features.localstorage) {
        if (!!code) {
            localStorage.setItem(LS_CODE, code);
        }
    }
}
/* Trigger when button clicked */
$('#code-run').on("click", function() {
    setTimeout(runUserCode, 200);
});
$('#terminal-clear').on("click", function() {
    document.terminal.clear();
});
$('#terminal-reset').on("click", function() {
    document.terminal.reset();
});
/* Save the editor contents to localStorage before leaving (if supported) */
$(window).on('unload', function() {
    saveCodeToLocalStorage();
});