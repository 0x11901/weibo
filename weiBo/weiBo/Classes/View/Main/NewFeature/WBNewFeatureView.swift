//
//  WBNewFeatureView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/27.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBNewFeatureView: UIScrollView {
    fileprivate lazy var pageControl:UIPageControl  = {
        let page:UIPageControl = UIPageControl()
        page.currentPage = 0
        page.numberOfPages = 4
        page.pageIndicatorTintColor = UIColor.lightGray
        page.currentPageIndicatorTintColor = globalColor
        return page
    }()

    override init(frame: CGRect) {
        super.init(frame: UIScreen.main.bounds)
        self.delegate = self
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

}

extension WBNewFeatureView {
    fileprivate func setupUI() {
        backgroundColor = UIColor.clear
        bounces = false
        isPagingEnabled = true
        showsVerticalScrollIndicator = false
        showsHorizontalScrollIndicator = false
        for i in 1...4{
            let image = UIImageView(imageName: "new_feature_\(i)")
            image.frame = self.frame.offsetBy(dx: CGFloat(i - 1) * bounds.size.width, dy: 0)
            image.contentMode = .scaleAspectFill
            addSubview(image)
        }
        contentSize = CGSize(width: 5 * bounds.size.width, height: bounds.size.height)
    }
}

extension WBNewFeatureView: UIScrollViewDelegate {
    func scrollViewDidScroll(_ scrollView: UIScrollView) {
        let tx = scrollView.contentOffset.x
        let page = Int(tx / screenWidth + 0.5)
        pageControl.currentPage = page
        
        if tx > (screenWidth * CGFloat(4.2)) {
            pageControl.isHidden = true
        }else{
            pageControl.isHidden = false
        }
        
        if page == 5 {
            UIView.animate(withDuration: 1.5, animations: { 
                self.removeFromSuperview()
            })
        }
    }
}
