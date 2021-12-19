// C# .NET string.format support
if (!String.prototype.format) {
    String.prototype.format = function() {
        var str = this.toString();
        if (!arguments.length)
            return str;
        var args = typeof arguments[0],
            args = (("string" == args || "number" == args) ? arguments : arguments[0]);
        for (arg in args)
            str = str.replace(RegExp("\\{" + arg + "\\}", "gi"), args[arg]);
        return str;
    }
}

// Get this year
function getFullYear() {
    var myDate = new Date();
    return myDate.getFullYear();
}

// Get copyright info
function getCopyrightInfo() {
    var info = "Copyright © 2014-{0} x-studio. All rights reserved.";
    return info.format(getFullYear());
}
