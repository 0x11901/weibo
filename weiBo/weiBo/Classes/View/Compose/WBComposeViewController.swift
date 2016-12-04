//
//  WBComposeViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.


import UIKit

class WBComposeViewController: UIViewController {
    
    fileprivate lazy var leftBarButton: UIBarButtonItem = {
        let left = UIButton(title: "取消", fontSize: 15, color: .white, bgImage: "new_feature_finish_button", target: self, action: #selector(cancell))
        left.frame = CGRect(x: 0, y: 0, width: 50, height: 26)
        return UIBarButtonItem(customView: left)
    }()
    
    fileprivate lazy var rightBarButton: UIButton = {
        let right = UIButton(title: "发布", fontSize: 15, color: .white, bgImage: "new_feature_finish_button", target: self, action: #selector(compose))
        right.frame = CGRect(x: 0, y: 0, width: 50, height: 30)
        right.setTitleColor(UIColor.gray, for: .disabled)
        right.setBackgroundImage(UIImage(named: "common_button_white_disable"), for: .disabled)
        return right
    }()
    
    fileprivate lazy var titleLabel: UILabel = {
        let title = UILabel()
        let attr = NSMutableAttributedString(string: "发布微博\n", attributes: [NSForegroundColorAttributeName : UIColor.black,NSFontAttributeName : UIFont.systemFont(ofSize: 16)])
        attr.append(NSAttributedString(string: "\(WBUserAccountModel.shared.screen_name!)", attributes: [NSFontAttributeName: UIFont.systemFont(ofSize: 12), NSForegroundColorAttributeName: UIColor.lightGray]))
        title.attributedText = attr
        title.numberOfLines = 0
        title.textAlignment = .center
        title.sizeToFit()
        return title
    }()
    
    fileprivate lazy var toolBar: UIToolbar = {
        let tool = UIToolbar()
        let itemDictArray: [[String: Any]] =
            [["image": "compose_toolbar_picture", "Selector": #selector(emotionKeyboard)],
            ["image": "compose_trendbutton_background", "Selector": #selector(emotionKeyboard)],
            ["image": "compose_mentionbutton_background", "Selector": #selector(emotionKeyboard)],
            ["image": "compose_emoticonbutton_background", "Selector": #selector(emotionKeyboard)],
            ["image": "compose_keyboardbutton_background", "Selector": #selector(emotionKeyboard)]]
        var array:[UIBarButtonItem] = []
        for dict in itemDictArray {
            let btn = UIButton(title: "", image: dict["image"] as? String, target: self, action: dict["Selector"] as? Selector)
            let btnItem = UIBarButtonItem(customView: btn)
            btn.sizeToFit()
            array.append(btnItem)
            let flex = UIBarButtonItem(barButtonSystemItem: UIBarButtonSystemItem.flexibleSpace, target: nil, action: nil)
            array.append(flex)
        }
        array.removeLast()
        tool.items = array
        return tool
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()

        setupUI()
    }

}

extension WBComposeViewController {
    
    fileprivate func setupUI() {
        view.backgroundColor = UIColor.randomColor()
        setNavigation()
        setTextField()
        setToolBar()
    }
    
    fileprivate func setNavigation() {
        navigationItem.leftBarButtonItem = leftBarButton
        navigationItem.rightBarButtonItem = UIBarButtonItem(customView: rightBarButton)
        rightBarButton.isEnabled = false
        navigationItem.titleView = titleLabel
    }
    
    fileprivate func setTextField() {
        let textView = WBComposeTextView()
        view.addSubview(textView)
        textView.snp.makeConstraints { (make) in
            make.edges.equalTo(view)
        }
    }
    
    fileprivate func setToolBar() {
        view.addSubview(toolBar)
        toolBar.snp.makeConstraints { (make) in
            make.leading.trailing.bottom.equalTo(view)
        }
    }
}

extension WBComposeViewController {
    
    @objc fileprivate func cancell() {
        dismiss(animated: true, completion: nil)
    }
    
    @objc fileprivate func compose() {
        print("hello world")
    }
    
    @objc fileprivate func emotionKeyboard() {
        print("hello world")
    }
    
}
