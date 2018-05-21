//
//  AXLabel.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/7.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

@objc
public protocol AXLabelDelegate: NSObjectProtocol {
    /// 选中链接文本
    ///
    /// - parameter label: label
    /// - parameter text:  选中的文本
    @objc optional func labelDidSelectedLinkText(label: AXLabel, text: String)
}

public class AXLabel: UILabel {
    public var linkTextColor = UIColor.colorWithHex(hex: 0x507DAF)
    public var selectedBackgroudColor = UIColor.lightGray
    public weak var delegate: AXLabelDelegate?

    // MARK: - override properties

    public override var text: String? {
        didSet {
            updateTextStorage()
        }
    }

    public override var attributedText: NSAttributedString? {
        didSet {
            updateTextStorage()
        }
    }

    public override var font: UIFont! {
        didSet {
            updateTextStorage()
        }
    }

    public override var textColor: UIColor! {
        didSet {
            updateTextStorage()
        }
    }

    // MARK: - upadte text storage and redraw text

    private func updateTextStorage() {
        if attributedText == nil {
            return
        }

        let attrStringM = addLineBreak(attributedText!)
        regexLinkRanges(attrStringM)
        addLinkAttribute(attrStringM)

        textStorage.setAttributedString(attrStringM)

        setNeedsDisplay()
    }

    /// add link attribute
    private func addLinkAttribute(_ attrStringM: NSMutableAttributedString) {
        if attrStringM.length == 0 {
            return
        }

        var range = NSRange(location: 0, length: 0)
        var attributes = attrStringM.attributes(at: 0, effectiveRange: &range)

        attributes[NSAttributedStringKey.font] = font!
        attributes[NSAttributedStringKey.foregroundColor] = textColor
        attrStringM.addAttributes(attributes, range: range)

        attributes[NSAttributedStringKey.foregroundColor] = linkTextColor

        for r in linkRanges {
            attrStringM.setAttributes(attributes, range: r)
        }
    }

    /// use regex check all link ranges
    private let patterns = ["[a-zA-Z]*://[a-zA-Z0-9/\\.]*", "#.*?#", "@[\\u4e00-\\u9fa5a-zA-Z0-9_-]*"]
    private func regexLinkRanges(_ attrString: NSAttributedString) {
        linkRanges.removeAll()
        let regexRange = NSRange(location: 0, length: attrString.string.count)

        for pattern in patterns {
            let regex = try! NSRegularExpression(pattern: pattern, options: NSRegularExpression.Options.dotMatchesLineSeparators)
            let results = regex.matches(in: attrString.string, options: NSRegularExpression.MatchingOptions(rawValue: 0), range: regexRange)

            for r in results {
                r.range(at: 0)
                linkRanges.append(r.range(at: 0))
            }
        }
    }

    /// add line break mode
    private func addLineBreak(_ attrString: NSAttributedString) -> NSMutableAttributedString {
        let attrStringM = NSMutableAttributedString(attributedString: attrString)

        if attrStringM.length == 0 {
            return attrStringM
        }

        var range = NSRange(location: 0, length: 0)
        var attributes = attrStringM.attributes(at: 0, effectiveRange: &range)
        var paragraphStyle = attributes[NSAttributedStringKey.paragraphStyle] as? NSMutableParagraphStyle

        if paragraphStyle != nil {
            paragraphStyle!.lineBreakMode = NSLineBreakMode.byWordWrapping
        } else {
            // iOS 8.0 can not get the paragraphStyle directly
            paragraphStyle = NSMutableParagraphStyle()
            paragraphStyle!.lineBreakMode = NSLineBreakMode.byWordWrapping
            attributes[NSAttributedStringKey.paragraphStyle] = paragraphStyle

            attrStringM.setAttributes(attributes, range: range)
        }

        return attrStringM
    }

    public override func drawText(in _: CGRect) {
        let range = glyphsRange()
        let offset = glyphsOffset(range)

        layoutManager.drawBackground(forGlyphRange: range, at: offset)
        layoutManager.drawGlyphs(forGlyphRange: range, at: CGPoint.zero)
    }

    private func glyphsRange() -> NSRange {
        return NSRange(location: 0, length: textStorage.length)
    }

    private func glyphsOffset(_ range: NSRange) -> CGPoint {
        let rect = layoutManager.boundingRect(forGlyphRange: range, in: textContainer)
        let height = (bounds.height - rect.height) * 0.5

        return CGPoint(x: 0, y: height)
    }

    // MARK: - touch events

    public override func touchesBegan(_ touches: Set<UITouch>, with _: UIEvent?) {
        let location = touches.first!.location(in: self)

        selectedRange = linkRangeAtLocation(location)
        modifySelectedAttribute(true)
    }

    public override func touchesMoved(_ touches: Set<UITouch>, with _: UIEvent?) {
        let location = touches.first!.location(in: self)

        if let range = linkRangeAtLocation(location) {
            if !(range.location == selectedRange?.location && range.length == selectedRange?.length) {
                modifySelectedAttribute(false)
                selectedRange = range
                modifySelectedAttribute(true)
            }
        } else {
            modifySelectedAttribute(false)
        }
    }

    public override func touchesEnded(_: Set<UITouch>, with _: UIEvent?) {
        if selectedRange != nil {
            let text = (textStorage.string as NSString).substring(with: selectedRange!)
            delegate?.labelDidSelectedLinkText?(label: self, text: text) // 使用代理传递太远，该用通知
            NotificationCenter.default.post(name: clickHyperlink, object: self, userInfo: [hyperlinkTextKey: text])

            let when = DispatchTime.now() + Double(Int64(0.25 * Double(NSEC_PER_SEC))) / Double(NSEC_PER_SEC)

            DispatchQueue.main.asyncAfter(deadline: when, execute: {
                self.modifySelectedAttribute(false)
            })
        }
    }

    public override func touchesCancelled(_: Set<UITouch>, with _: UIEvent?) {
        modifySelectedAttribute(false)
    }

    private func modifySelectedAttribute(_ isSet: Bool) {
        if selectedRange == nil {
            return
        }

        var attributes = textStorage.attributes(at: 0, effectiveRange: nil)
        attributes[NSAttributedStringKey.foregroundColor] = linkTextColor
        let range = selectedRange!

        if isSet {
            attributes[NSAttributedStringKey.backgroundColor] = selectedBackgroudColor
        } else {
            attributes[NSAttributedStringKey.backgroundColor] = UIColor.clear
            selectedRange = nil
        }

        textStorage.addAttributes(attributes, range: range)

        setNeedsDisplay()
    }

    private func linkRangeAtLocation(_ location: CGPoint) -> NSRange? {
        if textStorage.length == 0 {
            return nil
        }

        let offset = glyphsOffset(glyphsRange())
        let point = CGPoint(x: offset.x + location.x, y: offset.y + location.y)
        let index = layoutManager.glyphIndex(for: point, in: textContainer)

        for r in linkRanges {
            if index >= r.location && index <= r.location + r.length {
                return r
            }
        }

        return nil
    }

    // MARK: - init functions

    public override init(frame: CGRect) {
        super.init(frame: frame)

        prepareLabel()
    }

    public required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)

        prepareLabel()
    }

    public override func layoutSubviews() {
        super.layoutSubviews()

        textContainer.size = bounds.size
    }

    private func prepareLabel() {
        textStorage.addLayoutManager(layoutManager)
        layoutManager.addTextContainer(textContainer)

        textContainer.lineFragmentPadding = 0

        isUserInteractionEnabled = true
    }

    // MARK: lazy properties

    private lazy var linkRanges = [NSRange]()
    private var selectedRange: NSRange?
    private lazy var textStorage = NSTextStorage()
    private lazy var layoutManager = NSLayoutManager()
    private lazy var textContainer = NSTextContainer()
}

extension AXLabel {
    convenience init(title: String?, fontSize: CGFloat = 14, fontColor: UIColor = UIColor.black, textAlignment: NSTextAlignment = .natural, numberOfLines: NSInteger = 0) {
        self.init()
        text = title
        font = UIFont.systemFont(ofSize: fontSize)
        textColor = fontColor
        self.textAlignment = textAlignment
        self.numberOfLines = numberOfLines
    }
}
