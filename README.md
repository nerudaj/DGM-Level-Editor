# DGM Level Editor
 2D Level Editor made using DGM, SFML and TGUI

## How to use

After starting the editor, click the `File->New level` option. Enter whatever dimensions you want and link to the config file (you can use example `config.json`).

After hitting OK, you can pick tiles from right sidebar and draw them to highlighted part of the canvas using LMB. Use RMB to erase tiles. Use `Editor->Item mode` or `I` keyboard key to switch to Item placement mode, which works the same way as the Tiles mode.

You can edit properties (flags) of Items after switching to Properties mode and selecting the item with LMB.

You can then save the level (always specify the `.lvd` extension!) and load them afterwards.

> Always first setup new level (so the editor knows its config) and then load the new level!