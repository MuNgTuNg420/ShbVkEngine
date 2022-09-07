#include "sSwapchain.hpp"
namespace shb{


void sSwapchain::createSwapchain(){

    //querying swapchain-device details
    uint32_t surfaceFormatsCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(_device.getPhysicalDevice(),_device.getSurface(),&surfaceFormatsCount,nullptr);
    std::vector<VkSurfaceFormatKHR> formats(surfaceFormatsCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(_device.getPhysicalDevice(),_device.getSurface(),&surfaceFormatsCount,formats.data());


    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device.getPhysicalDevice(),_device.getSurface(),&surfaceCapabilities);
    QueueFamilyIndices QFI = _device.findQueueFamilies(_device.getPhysicalDevice());
    uint32_t QFIindeces[] = {QFI.graphicsFamily, QFI.presentFamily};

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(_device.getPhysicalDevice(),_device.getSurface(),&presentModeCount,nullptr);
    std::vector<VkPresentModeKHR>  presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(_device.getPhysicalDevice(),_device.getSurface(),&presentModeCount,presentModes.data());


    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType =  VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _device.getSurface();
    
    
    for(const auto& format : formats){
        
       if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR  ){
       createInfo.imageFormat = format.format;
       std::cout<< "Image format: " << "VK_FORMAT_B8G8R8A8_SRGB\n";
       createInfo.imageColorSpace = format.colorSpace;
        std::cout<< "Color space: " << "VK_COLOR_SPACE_SRGB_NONLINEAR_KHR\n";
        break;
       }
       
    }
    

    int image_count = surfaceCapabilities.minImageCount + 1;
    if(surfaceCapabilities.maxImageCount != 0 && image_count < surfaceCapabilities.maxImageCount){
        image_count = surfaceCapabilities.maxImageCount;
    }
    createInfo.minImageCount = surfaceCapabilities.minImageCount +1;
    
    createInfo.imageExtent = surfaceCapabilities.currentExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = QFIindeces;
    createInfo.preTransform = surfaceCapabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    for(const auto& modes : presentModes){
        if(modes == VK_PRESENT_MODE_MAILBOX_KHR){
            createInfo.presentMode = modes;
        }else if(modes == VK_PRESENT_MODE_FIFO_KHR){
            createInfo.presentMode = modes;
        }
        
    }

    createInfo.presentMode = _presentMode;

    createInfo.clipped = VK_TRUE;
    if(_oldSwapchain == nullptr){
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    }else{
    createInfo.oldSwapchain = _oldSwapchain->_swapchain;
    }
    
    if(vkCreateSwapchainKHR(_device.getDevice(),&createInfo,nullptr,&_swapchain) != VK_SUCCESS){
        std::cout<<"Failed to create swapchain\n";
    }else{
        std::cout<<"Created swapchain\n";
    }
    
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(_device.getDevice(),_swapchain,&imageCount,nullptr);
    _swapchainImages.resize(image_count);
    vkGetSwapchainImagesKHR(_device.getDevice(),_swapchain,&imageCount,_swapchainImages.data());
    std::cout<< "Swapchain Images: " << _swapchainImages.size() << std::endl;
    
}



}//namespace shb
