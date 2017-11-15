//
//  WBPhotoBrowserViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/3.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBPhotoBrowserViewController: UIViewController {
    let index: Int
    let images: [String]
    var pageViewController: UIPageViewController?
    
    init(index: Int,images: [String]) {
        self.index = index
        self.images = images
        super.init(nibName: nil, bundle: nil)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
    }

}

extension WBPhotoBrowserViewController {
    
    fileprivate func setupUI() {
        self.pageViewController = UIPageViewController(transitionStyle: .scroll, navigationOrientation: .horizontal, options: nil)
        let startingViewController = WBPhotoViewer(index: index, url: images[index])
        let viewControllers = [startingViewController]
        self.pageViewController!.setViewControllers(viewControllers, direction: .forward, animated: false, completion: nil)
        self.pageViewController!.dataSource = self
        self.addChildViewController(self.pageViewController!)
        self.view.addSubview(self.pageViewController!.view)
        self.pageViewController!.didMove(toParentViewController: self)

        let tap = UITapGestureRecognizer(target: self, action: #selector(tapAction))
        view.addGestureRecognizer(tap)
    }
    
}

extension WBPhotoBrowserViewController {
    
    @objc fileprivate func tapAction() {
        dismiss(animated: false, completion: nil)
    }
    
}

extension WBPhotoBrowserViewController: UIPageViewControllerDataSource {
    
    func pageViewController(_ pageViewController: UIPageViewController, viewControllerBefore viewController: UIViewController) -> UIViewController?{
        if let id = (viewController as? WBPhotoViewer)?.index {
            if id > 0 {
                 return WBPhotoViewer(index: id - 1, url: images[id - 1])
            }
        }
        return nil
    }
    
    func pageViewController(_ pageViewController: UIPageViewController, viewControllerAfter viewController: UIViewController) -> UIViewController?{
        if let id = (viewController as? WBPhotoViewer)?.index {
            if id < images.count - 1 {
                return WBPhotoViewer(index: id + 1, url: images[id + 1])
            }
        }
        return nil
    }
}















