//
//  WBRootViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBRootViewController: UITabBarController {
    
    /// 发布微博按钮
    fileprivate lazy var composeButton: UIButton = UIButton(title: nil,image: "tabbar_compose_icon_add", bgImage: "tabbar_compose_button", target: self, action: #selector(pushCompose))
    fileprivate lazy var newFeatureView: WBNewFeatureView = WBNewFeatureView()
    fileprivate lazy var welcomeView: WBWelcomeView = WBWelcomeView()
    
    override func viewDidLoad() {
        setupUI()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        tabBar.bringSubview(toFront: composeButton)
    }
}

// MARK: - UI相关
extension WBRootViewController {
    fileprivate func setupUI() {
        setupTabBar()
        if Bundle.isNewFeature() {
            setupNewFeature()
        }
        if WBUserAccountModel.shared.isLogin {
            setupWelcome()
        }
    }
    
    fileprivate func setupNewFeature() {
        view.addSubview(newFeatureView)
    }
    
    fileprivate func setupWelcome() {
        view.addSubview(welcomeView)
    }
    
    fileprivate func setupTabBar() {
        if let url = Bundle.main.url(forResource: "main", withExtension: "json"){
            if let json = try? Data(contentsOf: url){
                if let array = try? JSONSerialization.jsonObject(with: json, options: []) as! [[String : Any]] {
                    for dict in array{
                        if let clsName = dict["clsName"] as? String{
                            let title = dict["title"] as? String
                            let image = dict["imageName"] as? String
                            let dict = dict["visitorInfo"] as? [String : Any]
                            var anima = false
                            if clsName == "WBHomeViewController" {
                                anima = true
                            }
                            addChildViewController(childController: clsName, title: title, image: image, dictionary: dict, isAnimation: anima)
                        }
                    }
                }
            }
        }
    }
    
    fileprivate func addChildViewController(childController: String, title: String?, image: String?, dictionary: [String : Any]?, isAnimation: Bool){
        if let vcClass = NSClassFromString(Bundle.main.infoDictionary?["CFBundleName"] as! String + "." + childController) as? WBBaseViewController.Type{
            let controller = vcClass.init()
            
            if let title = title {
                controller.title = title
            }
            if var image = image {
                image = "tabbar_\(image)"
                controller.tabBarItem.image = UIImage(named: image)?.withRenderingMode(.alwaysOriginal)
                controller.tabBarItem.selectedImage = UIImage(named: image + "_selected")?.withRenderingMode(.alwaysOriginal)
            }
            
            if let dictionary = dictionary {
                let model = WBVisitorModel(dictionary: dictionary)
                if isAnimation == true {
                    model.isAnima = true
                }
                controller.model = model
            }
            
            addChildViewController(UINavigationController(rootViewController: controller))
            addComposeButton()
        }
    }
    
    fileprivate func addComposeButton() {
        tabBar.addSubview(composeButton)
        composeButton.center = CGPoint(x: tabBar.center.x, y: tabBar.bounds.size.height * 0.5)
//        composeButton.sizeToFit()
        let frame = tabBar.bounds
        composeButton.frame = frame.insetBy(dx: screenWidth/5 * 2 - 1, dy: 5)
    }
}


// MARK: - 响应事件
extension WBRootViewController {
    @objc fileprivate func pushCompose() {
        let composeController = WBComposeViewController()
        let composeNav = UINavigationController(rootViewController: composeController)
        present(composeNav, animated: true, completion: nil)
    }
}
