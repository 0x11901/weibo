//
//  WBPhotoBrowserViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/3.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import ImageViewer
import Kingfisher

extension UIImageView: DisplaceableView {}

class WBPhotoBrowserViewController {
    struct DataItem {
        let imageView: UIImageView
        let galleryItem: GalleryItem
    }
    
    let index: Int
    let images: [String]
    var items: [DataItem] = []
    var imageViews: [UIImageView] = []
    
    /// 使用UIPageViewController的方案
    var pageViewController: UIPageViewController?
    lazy var galleryViewController: GalleryViewController = {
        let galleryViewController = GalleryViewController(startIndex: index, itemsDataSource: self, itemsDelegate: self, displacedViewsDataSource: self, configuration: galleryConfiguration())
        
        //        galleryViewController.launchedCompletion = { print("LAUNCHED") }
        //        galleryViewController.closedCompletion = { print("CLOSED") }
        //        galleryViewController.swipedToDismissCompletion = { print("SWIPE-DISMISSED") }
        //        galleryViewController.landedPageAtIndexCompletion = { index in print("LANDED AT INDEX: \(index)") }
        
        return galleryViewController
    }()
    
    init(index: Int,images: [String]) {
        self.index = index
        self.images = images
        for element in images.enumerated() {
            let image = element.element
            let imageView = UIImageView()
            imageView.setImage(urlStr: image)
            var galleryItem: GalleryItem!
            if let imageRef = imageView.image {
                galleryItem = GalleryItem.image{ $0(imageRef) }
            }else{
                galleryItem = GalleryItem.image(fetchImageBlock: { (imageCompletion) in
                    guard let url = URL(string: image) else {
                        imageCompletion(UIImage(named: "new_feature_4"))
                        return
                    }
                    ImageDownloader.default.downloadImage(with: url, retrieveImageTask: nil, options: nil, progressBlock: { (r, t) in
                        print("\(r/t)")
                    }, completionHandler: { (image, error, _, _) in
                        if error == nil {
                            imageCompletion(image)
                        }else{
                            imageCompletion(UIImage(named: "new_feature_4"))
                        }
                    })
                })
            }
            
            
            items.append(DataItem(imageView: imageView, galleryItem: galleryItem))
            imageViews.append(imageView)
        }
    }
}

extension WBPhotoBrowserViewController {
    
    private func galleryConfiguration() -> GalleryConfiguration {
        
        return [
            
            GalleryConfigurationItem.closeButtonMode(.builtIn),
            GalleryConfigurationItem.seeAllCloseButtonMode(.none),
            GalleryConfigurationItem.deleteButtonMode(.none),
            GalleryConfigurationItem.thumbnailsButtonMode(.none),
            
            GalleryConfigurationItem.hideDecorationViewsOnLaunch(true),
            
            GalleryConfigurationItem.pagingMode(.standard),
            GalleryConfigurationItem.presentationStyle(.displacement),
            GalleryConfigurationItem.hideDecorationViewsOnLaunch(false),
            
            GalleryConfigurationItem.swipeToDismissMode(.vertical),
            GalleryConfigurationItem.toggleDecorationViewsBySingleTap(false),
            GalleryConfigurationItem.activityViewByLongPress(false),
            
            GalleryConfigurationItem.overlayColor(UIColor(white: 0.035, alpha: 1)),
            GalleryConfigurationItem.overlayColorOpacity(1),
            GalleryConfigurationItem.overlayBlurOpacity(1),
            GalleryConfigurationItem.overlayBlurStyle(UIBlurEffectStyle.light),
            
            GalleryConfigurationItem.videoControlsColor(.white),
            
            GalleryConfigurationItem.maximumZoomScale(16),
            GalleryConfigurationItem.swipeToDismissThresholdVelocity(500),
            
            GalleryConfigurationItem.doubleTapToZoomDuration(0.15),
            
            GalleryConfigurationItem.blurPresentDuration(0.5),
            GalleryConfigurationItem.blurPresentDelay(0),
            GalleryConfigurationItem.colorPresentDuration(0.25),
            GalleryConfigurationItem.colorPresentDelay(0),
            
            GalleryConfigurationItem.blurDismissDuration(0.1),
            GalleryConfigurationItem.blurDismissDelay(0.4),
            GalleryConfigurationItem.colorDismissDuration(0.45),
            GalleryConfigurationItem.colorDismissDelay(0),
            
            GalleryConfigurationItem.itemFadeDuration(0.3),
            GalleryConfigurationItem.decorationViewsFadeDuration(0.15),
            GalleryConfigurationItem.rotationDuration(0.15),
            
            GalleryConfigurationItem.displacementDuration(0.55),
            GalleryConfigurationItem.reverseDisplacementDuration(0.25),
            GalleryConfigurationItem.displacementTransitionStyle(.springBounce(0.7)),
            GalleryConfigurationItem.displacementTimingCurve(.linear),
            
            GalleryConfigurationItem.statusBarHidden(true),
            GalleryConfigurationItem.displacementKeepOriginalInPlace(false),
            GalleryConfigurationItem.displacementInsetMargin(50)
        ]
    }
}

extension WBPhotoBrowserViewController: GalleryDisplacedViewsDataSource {
    
    func provideDisplacementItem(atIndex index: Int) -> DisplaceableView? {
        
        return index < items.count ? items[index].imageView : nil
    }
}

extension WBPhotoBrowserViewController: GalleryItemsDataSource {
    
    func itemCount() -> Int {
        
        return items.count
    }
    
    func provideGalleryItem(_ index: Int) -> GalleryItem {
        
        return items[index].galleryItem
    }
}

extension WBPhotoBrowserViewController: GalleryItemsDelegate {
    
    func removeGalleryItem(at index: Int) {
        
        //        print("remove item at \(index)")
        //
        //        let imageView = items[index].imageView
        //        imageView.removeFromSuperview()
        //        items.remove(at: index)
    }
}

//// MARK: - 使用UIPageViewController的方案
//extension WBPhotoBrowserViewController {
//
//    fileprivate func setupUI() {
//        self.pageViewController = UIPageViewController(transitionStyle: .scroll, navigationOrientation: .horizontal, options: nil)
//        let startingViewController = WBPhotoViewer(index: index, url: images[index])
//        let viewControllers = [startingViewController]
//        self.pageViewController!.setViewControllers(viewControllers, direction: .forward, animated: false, completion: nil)
//        self.pageViewController!.dataSource = self
//        self.addChildViewController(self.pageViewController!)
//        self.view.addSubview(self.pageViewController!.view)
//        self.pageViewController!.didMove(toParentViewController: self)
//
//        let tap = UITapGestureRecognizer(target: self, action: #selector(tapAction))
//        view.addGestureRecognizer(tap)
//    }
//
//}
//
//extension WBPhotoBrowserViewController {
//
//    @objc fileprivate func tapAction() {
//        dismiss(animated: false, completion: nil)
//    }
//
//}
//
//extension WBPhotoBrowserViewController: UIPageViewControllerDataSource {
//
//    func pageViewController(_ pageViewController: UIPageViewController, viewControllerBefore viewController: UIViewController) -> UIViewController?{
//        if let id = (viewController as? WBPhotoViewer)?.index {
//            if id > 0 {
//                return WBPhotoViewer(index: id - 1, url: images[id - 1])
//            }
//        }
//        return nil
//    }
//
//    func pageViewController(_ pageViewController: UIPageViewController, viewControllerAfter viewController: UIViewController) -> UIViewController?{
//        if let id = (viewController as? WBPhotoViewer)?.index {
//            if id < images.count - 1 {
//                return WBPhotoViewer(index: id + 1, url: images[id + 1])
//            }
//        }
//        return nil
//    }
//}















