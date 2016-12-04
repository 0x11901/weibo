//
//  WBComposeTextView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/3.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBComposeTextView: UITextView {
    override var font: UIFont?{
        didSet{
            placeHolderLabel.font = font
        }
    }
    
    var placeHolder: String?{
        didSet{
            placeHolderLabel.text = placeHolder
        }
    }
    
    fileprivate lazy var placeHolderLabel: UILabel = {
        let ph = UILabel(title: "请输入你想说的话", fontSize: 14, fontColor: UIColor.darkGray)
        return ph
    }()
    
    override init(frame: CGRect, textContainer: NSTextContainer?) {
        super.init(frame: frame, textContainer: textContainer)
        NotificationCenter.default.addObserver(self, selector: #selector(textDidChange), name: NSNotification.Name.UITextViewTextDidChange, object: nil)
        setupView()
    }
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

}

extension WBComposeTextView {
    
    fileprivate func setupView() {
        backgroundColor = UIColor.white
        alwaysBounceVertical = true
        keyboardDismissMode = .onDrag
        setupTextField()
    }
    
    fileprivate func setupTextField() {
        addSubview(placeHolderLabel)
        placeHolderLabel.translatesAutoresizingMaskIntoConstraints = false
        addConstraints(NSLayoutConstraint.constraints(withVisualFormat: "H:|-5-[ph]-5-|", options: [], metrics: nil, views: ["ph" : placeHolderLabel]))
        addConstraints(NSLayoutConstraint.constraints(withVisualFormat: "V:|-8-[ph]", options: [], metrics: nil, views: ["ph" : placeHolderLabel]))
    }
}

extension WBComposeTextView {
    
    @objc fileprivate func textDidChange() {
        placeHolderLabel.isHidden = text.characters.count > 0
    }
    
}



















