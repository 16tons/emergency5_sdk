The LibRocket password widget didn't work correctly. 

For every glyph that was entered appears a "*", but if the entered glyph needs more than one byte in memory it appears one extra "*" sign for every extra needed byte ("a" => "*", "ü" => "**"). The change in the widget class fixes this bug.