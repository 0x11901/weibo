//
//  WBRootViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBRootViewController: UITabBarController {
    fileprivate lazy var composeButton: UIButton = UIButton(title: nil,image: "tabbar_compose_icon_add", bgImage: "tabbar_compose_button", target: self, action: #selector(pushCompose))
    
    override func viewDidLoad() {
        setupUI()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        tabBar.bringSubview(toFront: composeButton)
        composeButton.sizeThatFits(CGSize(width: tabBar.itemWidth, height: tabBar.bounds.size.height))
        print(tabBar.itemWidth,tabBar.bounds.size.height,composeButton)
    }
}

// MARK: - UI相关
extension WBRootViewController {
    func setupUI() {
        view.backgroundColor = UIColor.randomColor()
        setupTabBar()
    }
    
    func setupTabBar() {
        if let url = Bundle.main.url(forResource: "main", withExtension: "json"){
            if let json = try? Data(contentsOf: url){
                if let array = try? JSONSerialization.jsonObject(with: json, options: []) as! [[String : Any]] {
                    for dict in array{
                        if let clsName = dict["clsName"] as? String{
                            let title = dict["title"] as? String
                            let image = dict["imageName"] as? String
                            addChildViewController(childController: clsName, title: title, image: image)
                        }
                    }
                }
            }
        }
    }
    
    func addChildViewController(childController: String, title: String?, image: String?){
        if let vcClass = NSClassFromString(Bundle.main.infoDictionary?["CFBundleName"] as! String + "." + childController) as? UIViewController.Type{
            let controller = vcClass.init()
           
            if let title = title {
                controller.title = title
            }
            if var image = image {
                image = "tabbar_\(image)"
                controller.tabBarItem.image = UIImage(named: image)?.withRenderingMode(.alwaysOriginal)
                controller.tabBarItem.selectedImage = UIImage(named: image + "_selected")?.withRenderingMode(.alwaysOriginal)
            }
            addChildViewController(UINavigationController(rootViewController: controller))
            addComposeButton()
        }
    }
    
    func addComposeButton() {
        tabBar.addSubview(composeButton)
        composeButton.center = CGPoint(x: tabBar.center.x, y: tabBar.bounds.size.height * 0.5)
        composeButton.sizeToFit()
    }
}


// MARK: - 响应事件
extension WBRootViewController {
    @objc fileprivate func pushCompose() {
        print("hello world")
    }
}
